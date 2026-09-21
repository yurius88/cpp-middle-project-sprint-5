#include "convex_hull.hpp"
#include "geometry.hpp"
#include "intersections.hpp"
#include "queries.hpp"
#include "shape_utils.hpp"
#include "triangulation.hpp"
#include "visualization.hpp"

#include <algorithm>
#include <optional>
#include <print>
#include <random>
#include <ranges>

using namespace geometry;

namespace rng = std::ranges;
namespace views = std::ranges::views;

void PrintAllIntersections(const Shape &shape, std::span<const Shape> others) {
    std::println("\n=== Intersections ===");

    /*
     * Используйте ranges чтобы оставить только фигуры,
     * поддерживающие возможность находить пересечения между собой
     *
     * Затем примените монадический интерфейс для обработки результатов:
     *     - Пересечение найдено в точке A между фигурами B и C
     *     - Фигуры B и C не пересекаются
     */

    auto filtered_shapes = others | views::filter([&](const Shape &item) {
                               try {
                                   std::visit(geometry::intersections::IntersectionVisitor{}, shape, item);
                                   return !(shape == item);  // пересечение с самим собой не допускается

                               } catch (...) {
                                   return false;
                               }
                           });
    for (auto s : filtered_shapes) {
        std::make_optional(s)
            .and_then([&](const Shape &item) -> std::optional<Point2D> {
                auto result = std::visit(geometry::intersections::IntersectionVisitor{}, shape, item);
                if (result.has_value()) {
                    std::println("Пересечение найдено в точке {} между фигурами {} и {}", result.value(), shape, s);
                    return result;
                }
                return std::nullopt;
            })
            .or_else([&]() -> std::optional<Point2D> {
                std::println("Фигуры {} и {} не пересекаются", shape, s);
                return std::nullopt;
            });
    }
}

void PrintDistancesFromPointToShapes(Point2D p, std::span<const Shape> shapes) {
    std::println("\n=== Distance from Point Test ===");

    /*
     * Используйте ranges чтобы выбрать любые 5 фигур из списка.
     * Затем найдите расстояния от заданной точки до всех выбранных фигур.
     * Выведите результат в формате "Расстояние от точки P до фигуры S равно D"
     */

    const int sample_size = std::min(static_cast<int>(shapes.size()), 5);
    auto indices_view = std::views::iota(0, sample_size);
    std::vector<int> all_indices(indices_view.begin(), indices_view.end());

    std::vector<int> random_indices;
    std::random_device rd;
    std::mt19937 gen(rd());

    std::ranges::sample(all_indices, std::back_inserter(random_indices), 5, gen);

    auto random_shapes = random_indices | std::views::transform([&shapes](size_t idx) { return shapes[idx]; });
    for (const auto &s : random_shapes) {
        std::println("Расстояние от точки {} до фигуры {} равно {}", p, s, geometry::queries::DistanceToPoint(s, p));
    }
}

void PerformShapeAnalysis(std::span<const Shape> shapes) {
    std::println("\n=== Shape Analysis ===");

    /*
     * Используйте ranges и созданные классы чтобы:
     *     - Найти все пересечения между фигурами используя метод Bounding Box
     *     - Найти самую высокую фигуру (чья высота наибольшая)expected
     *     - Вывести расстояние между любыми двумя фигурами, которые поддерживают данную функциональность
     */

    auto all_collisions = utils::FindAllCollisions(shapes);
    for (const auto &[shape1, shape2] : all_collisions) {
        std::println("Найдено пересечение между {} и {}", shape1, shape2);
    }

    auto max_shape = utils::FindHighestShape(shapes);
    if (max_shape.has_value())
        std::println("Высота самой высокой фигуры {}", max_shape.value());
    else
        std::println("Нет доступных фигур для анализа");

    for (const auto &[pair, distance] :
         all_collisions |
             views::transform([](const auto &pair) -> std::tuple<std::pair<Shape, Shape>, std::optional<double>> {
                 return {pair, geometry::queries::DistanceBetweenShapes(pair.first, pair.second)};
             })) {
        if (distance.has_value()) {
            std::println("Расстояние между фигурой {} и {} равно {}", pair.first, pair.second, distance.value());
            break;
        }
    }
}

void PerformExtraShapeAnalysis(std::span<const Shape> shapes) {
    std::println("\n=== Shape Extra Analysis ===");

    /*
     * Используйте ranges и созданные классы чтобы:
     *     - Вывести 3 любые фигуры, которые находятся выше 50.0
     *     - Вывести фигуры с наименьшей и с наибольшей высотами
     */
    const double height = 50;
    std::println("Фигуры выше {}:", height);
    for (const auto &s : shapes | views::filter([&](const Shape &shape) {
                             return geometry::queries::GetHeight(shape) > height;
                         }) | views::take(3)) {
        std::println("{}", s);
    }

    auto [min_it, max_it] = rng::minmax_element(shapes, {}, &geometry::queries::GetHeight);
    if (min_it != shapes.end() && max_it != shapes.end()) {
        std::println("Минимальная фигура: {}", *min_it);
        std::println("Максимальная фигура: {}", *max_it);
    }
}

int main() {
    std::vector<Shape> shapes = utils::ParseShapes("circle 0 0 1.5; line 1 2 3 4; polygon 0 0 2 5; triangle 0 0 1 0 "
                                                   "0.5 1; polygon 0 0 1 2; badshape; circle 0 0 -1");
    std::println("Parsed {} shapes", shapes.size());

    // Выведите индекс каждой фигуры и её высоту
    for (auto const &[index, data] : std::views::enumerate(shapes)) {
        std::println("Shape {}: Index {} - Height {}", data, index, queries::GetHeight(data));
    }

    //
    // Вызываем разработанные функции
    //
    PrintAllIntersections(shapes[1], shapes);

    PrintDistancesFromPointToShapes(Point2D{10.0, 10.0}, shapes);

    PerformShapeAnalysis(shapes);

    PerformExtraShapeAnalysis(shapes);

    //
    // Рисуем все фигуры
    //
    // Важно: после изучения графика - нажмите Enter чтобы продолжить выполнение и построить 2ой график
    //
    geometry::visualization::Draw(shapes);

    //
    // Формируем список из вершин всех фигур
    //
    std::vector<Point2D> points;

    // /* ваш код здесь */
    auto fill_verticies = [&](const Shape &shape) {
        shape.visit(geometry::queries::Multilambda{
            [&](const Line &line) { points.append_range(line.Vertices()); },
            [&](const Triangle &triangle) { points.append_range(triangle.Vertices()); },
            [&](const Rectangle &rectangle) { points.append_range(rectangle.Vertices()); },
            [&](const RegularPolygon &regularPolygon) { points.append_range(regularPolygon.Vertices()); },
            [&](const Circle &circle) { points.append_range(circle.Vertices()); },
            [&](const Polygon &polygon) { points.append_range(polygon.Vertices()); },
        });
    };

    std::ranges::for_each(shapes, fill_verticies);

    //
    // Находим список точек, для построения выпуклой оболочки - convex hull - алгоритмом Грэхема
    // Создаём из них объект класса `Polygon` и добавляем его в список shapes
    // Рисуем все фигуры
    //

    // /* ваш код здесь */

    auto graham_points = convex_hull::GrahamScan(points);
    if (graham_points.has_value()) {
        auto &v = graham_points.value();
        if (!v.empty())
            v.erase(v.begin());
        shapes.emplace_back(Polygon(std::move(v)));
    }

    geometry::visualization::Draw(shapes);

    //
    // после изучения графика - нажмите Enter чтобы продолжить выполнение и построить 3ий график
    //

    {
        std::vector<Point2D> points = {{0, 0}, {10, 0}, {5, 8}, {15, 5}, {2, 12}};

        //
        // Используйте список точек points или свой, чтобы
        // выполнить алгоритм триангуляции Делоне алгоритмом Боуэра-Ватсона
        //

        // После успешного завершения алгоритма - выведите результат для проверки
        // используя geometry::visualization::Draw
        //

        auto triangulation = geometry::triangulation::DelaunayTriangulation(points);
        if (triangulation.has_value()) {
            shapes.clear();
            std::println("{}", triangulation.value());
            for (const auto &t : triangulation.value()) {

                auto vertices = t.vertices();
                Triangle triangle(vertices[0], vertices[1], vertices[2]);
                shapes.emplace_back(std::move(triangle));
            }

            geometry::visualization::Draw(shapes);
        }
    }
    return 0;
}