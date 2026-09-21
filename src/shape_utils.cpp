#include "shape_utils.hpp"
#include "geometry.hpp"
#include "queries.hpp"
#include <algorithm>
#include <functional>
#include <optional>

namespace geometry::utils {
// Разбивает строку на слова (по пробелам), игнорируя лишние пробелы
std::vector<std::string_view> SplitIntoWords(std::string_view s) {
    std::vector<std::string_view> words;
    size_t start = 0;
    size_t end = 0;

    while (start < s.size()) {
        // Пропускаем пробелы
        while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start])))
            ++start;
        if (start >= s.size())
            break;
        end = start;
        while (end < s.size() && !std::isspace(static_cast<unsigned char>(s[end])))
            ++end;
        words.push_back(s.substr(start, end - start));
        start = end;
    }
    return words;
}

// Безопасный парсинг строки в double (без исключений)
std::optional<double> ParseDouble(std::string_view s) {
    double value = 0.0;
    auto result = std::from_chars(s.data(), s.data() + s.size(), value);
    if (result.ec == std::errc{} && result.ptr == s.data() + s.size()) {
        return value;
    }
    return std::nullopt;
}

// Парсит строку в вектор double
std::optional<std::vector<double>> ParseDoubles(std::string_view s) {
    if (s.empty())
        return std::nullopt;
    auto tokens = SplitIntoWords(s);
    if (tokens.empty())
        return std::nullopt;

    std::vector<double> result;
    result.reserve(tokens.size());

    for (auto token : tokens) {
        auto num = ParseDouble(token);
        if (!num.has_value()) {
            return std::nullopt;
        }
        result.push_back(*num);
    }
    return result;
}

// Проверяет размер вектора и возвращает его, если совпадает
std::optional<std::vector<double>> RequireSize(const std::vector<double> &v, size_t expected) {
    return (v.size() == expected) ? std::make_optional(v) : std::nullopt;
}

// Проверяет, что значение > 0
std::optional<double> RequirePositive(double x) { return (x > 0) ? std::make_optional(x) : std::nullopt; }

// Проверяет, что double представляет целое число >= min_value
std::optional<int> RequireIntegerAtLeast(double d, int min_value) {
    int i = static_cast<int>(d);
    if (static_cast<double>(i) == d && i >= min_value) {
        return i;
    }
    return std::nullopt;
}

// Конструкторы фигур

/**
    @brief Создаёт круг из параметров
    @note Пример того как могла бы выглядеть эта функция:
        if (v.size() != 3) return std::nullopt;
        if (v[2] <= 0) return std::nullopt; // радиус должен быть > 0
        return Circle{{v[0], v[1]}, v[2]};
*/
std::optional<Shape> MakeCircle(const std::vector<double> &v) {
    return RequireSize(v, 3)
        .and_then([](const auto &shape) -> std::optional<std::vector<double>> {
            if (RequirePositive(shape[2]).has_value())
                return shape;
            return std::nullopt;
        })
        .transform([](const std::vector<double> &shape) { return Circle(Point2D(shape[0], shape[1]), shape[2]); });
}

/**
    @brief Создаёт линию из параметров
    @note Пример того как могла бы выглядеть эта функция:
        if (v.size() != 4) return std::nullopt;
        return Line{{v[0], v[1]}, {v[2], v[3]}};
*/
std::optional<Shape> MakeLine(const std::vector<double> &v) {
    return RequireSize(v, 4).transform(
        [](const std::vector<double> &shape) { return Line{{shape[0], shape[1]}, {shape[2], shape[3]}}; });
}
/**
    @brief Создаёт треугольник из параметров
    @note Пример того как могла бы выглядеть эта функция:
        if (v.size() != 6) return std::nullopt;
        return Triangle{{v[0], v[1]}, {v[2], v[3]}, {v[4], v[5]}};
*/
std::optional<Shape> MakeTriangle(const std::vector<double> &v) {
    return RequireSize(v, 6).transform([](const std::vector<double> &shape) {
        return Triangle{{shape[0], shape[1]}, {shape[2], shape[3]}, {shape[4], shape[5]}};
    });
}

/**
    @brief Создаёт прямоугольник из параметров
    @note Пример того как могла бы выглядеть эта функция:
        if (v.size() != 4) return std::nullopt;
        if (v[2] <= 0 || v[3] <= 0) return std::nullopt; // ширина/высота > 0
        return Rectangle{{v[0], v[1]}, v[2], v[3]};
*/
std::optional<Shape> MakeRectangle(const std::vector<double> &v) {
    return RequireSize(v, 4)
        .and_then([](const std::vector<double> &shape) -> std::optional<std::vector<double>> {
            if (RequirePositive(shape[2]).has_value() && RequirePositive(shape[3]).has_value())
                return shape;
            return std::nullopt;
        })
        .transform(
            [](const std::vector<double> &shape) { return Rectangle({shape[0], shape[1]}, shape[2], shape[3]); });
}

/**
    @brief Создаёт правильный многоугольник из параметров
    @note Пример того как могла бы выглядеть эта функция:
        if (v.size() != 4) return std::nullopt;
        if (v[2] <= 0) return std::nullopt; // радиус > 0
        auto sides_opt = parse_double(std::to_string(v[3])); // v[3] — double, но sides — целое
        if (!sides_opt.has_value()) return std::nullopt;
        int sides = static_cast<int>(v[3]);
        if (sides != v[3] || sides < 3) return std::nullopt; // должно быть целым и >=3
        return RegularPolygon{{v[0], v[1]}, v[2], sides};
*/
std::optional<Shape> MakePolygon(const std::vector<double> &v) {
    return RequireSize(v, 4)
        .and_then([](const std::vector<double> &shape) -> std::optional<std::vector<double>> {
            if (RequirePositive(shape[2]).has_value())
                return shape;
            return std::nullopt;
        })
        .and_then([](const std::vector<double> &shape) -> std::optional<Shape> {
            auto sides_opt = ParseDouble(std::to_string(shape[3]));
            if (!sides_opt.has_value())
                return std::nullopt;
            int sides = static_cast<int>(shape[3]);
            if (sides != shape[3] || sides < 3)
                return std::nullopt;

            return RegularPolygon({shape[0], shape[1]}, shape[2], sides);
        });
}

// Парсинг одной фигуры
std::optional<Shape> ParseSingleShape(std::string_view token) {
    auto parts = SplitIntoWords(token);
    if (parts.empty())
        return std::nullopt;

    std::string_view type = parts[0];
    std::string param_str;
    for (auto i : std::views::iota(1u, parts.size())) {
        if (!param_str.empty())
            param_str += ' ';
        param_str += std::string(parts[i]);
    }

    // Выбираем конструктор по имени
    auto get_maker =
        [](std::string_view t) -> std::optional<std::function<std::optional<Shape>(const std::vector<double> &)>> {
        if (t == "circle")
            return MakeCircle;
        if (t == "line")
            return MakeLine;
        if (t == "triangle")
            return MakeTriangle;
        if (t == "rectangle")
            return MakeRectangle;
        if (t == "polygon")
            return MakePolygon;
        return std::nullopt;
    };

    // Обратите внимание на код ниже
    return get_maker(type).and_then([&](auto maker) { return ParseDoubles(param_str).and_then(maker); });
}

std::vector<Shape> ParseShapes(std::string_view input) {
    std::vector<Shape> result;

    // Разделяем по ';'
    size_t start = 0;
    size_t end = 0;
    while (start < input.size()) {
        end = input.find(';', start);
        if (end == std::string_view::npos)
            end = input.size();

        std::string_view token = input.substr(start, end - start);
        // Убираем пробелы по краям
        while (!token.empty() && std::isspace(static_cast<unsigned char>(token.front())))
            token.remove_prefix(1);
        while (!token.empty() && std::isspace(static_cast<unsigned char>(token.back())))
            token.remove_suffix(1);

        if (!token.empty()) {
            auto shape_opt = ParseSingleShape(token);
            if (shape_opt.has_value()) {
                result.push_back(*shape_opt);
            }
        }

        start = end + 1;
    }

    return result;
}

std::vector<std::pair<Shape, Shape>> FindAllCollisions(std::span<const Shape> shapes) {
    std::vector<std::pair<Shape, Shape>> collisions;

    /*
     * Используйте библиотеку ranges, чтобы найти все коллизии между фигурами методом BoundingBoxesOverlap
     *
     * Также используйте наиболее эффективный метод добавления объектов в collisions
     */

    std::vector<std::pair<Shape, Shape>> container;
    auto indices = std::views::iota(0u, shapes.size());
    auto intersections = std::views::cartesian_product(indices, indices) | std::views::filter([](const auto &pair) {
                             auto [i, j] = pair;
                             return i < j;  // исключаем дубликаты
                         }) |
                         std::views::filter([&shapes](const auto &pair) {
                             auto [i, j] = pair;
                             return geometry::queries::BoundingBoxesOverlap(shapes[i], shapes[j]);
                         });
    for (auto [i, j] : intersections) {
        collisions.push_back(
            {std::piecewise_construct, std::forward_as_tuple(shapes[i]), std::forward_as_tuple(shapes[j])});
    }

    return collisions;
}

std::optional<size_t> FindHighestShape(std::span<const Shape> shapes) {
    /*
     * Используйте библиотеку ranges, чтобы найти самую высокую фигуру
     *
     * Важно: использование ручной итерации по фигурам не разрешается
     */

    auto element = std::ranges::max_element(shapes, {}, &queries::GetHeight);
    if (element != shapes.end())
        return queries::GetHeight(*element);

    return std::nullopt;
}
}  // namespace geometry::utils