#pragma once
#include "geometry.hpp"
#include <cmath>
#include <optional>
#include <stdexcept>

namespace geometry::intersections {

/*
 * Класс для поиска пересечений между двумя фигурами
 *
 * Требуется организовать возможность нахождения пересечений только для следующих комбинаций фигур:
 *    - Line   & Line
 *    - Line   & Circle
 *    - Circle & Circle
 *
 * Для всех остальных требуется выбросить исключение std::logic_error
 */

// Пересечение двух линий
inline std::optional<Point2D> GetIntersectPoint(const Line &line1, const Line &line2) {
    Point2D p1 = line1.start;
    Point2D p2 = line1.end;
    Point2D p3 = line2.start;
    Point2D p4 = line2.end;

    double x1 = p1.x, y1 = p1.y;
    double x2 = p2.x, y2 = p2.y;
    double x3 = p3.x, y3 = p3.y;
    double x4 = p4.x, y4 = p4.y;

    double denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    if (std::abs(denom) < 1e-10) {
        return std::nullopt;  // Линии параллельны
    }

    double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denom;
    double u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denom;

    if (t >= 0.0 && t <= 1.0 && u >= 0.0 && u <= 1.0) {
        double x = x1 + t * (x2 - x1);
        double y = y1 + t * (y2 - y1);
        return Point2D{x, y};
    }

    return std::nullopt;
}

// Пересечение линии и круга
inline std::optional<Point2D> GetIntersectPoint(const Line &line, const Circle &circle) {
    Point2D d = line.end - line.start;
    Point2D f = line.start - circle.center_p;

    double a = d.Dot(d);
    double b = 2 * f.Dot(d);
    double c = f.Dot(f) - circle.radius * circle.radius;

    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return std::nullopt;  // Нет пересечения
    }

    discriminant = std::sqrt(discriminant);

    double t1 = (-b - discriminant) / (2 * a);
    double t2 = (-b + discriminant) / (2 * a);

    if (t1 >= 0.0 && t1 <= 1.0) {
        return line.start + d * t1;
    }

    if (t2 >= 0.0 && t2 <= 1.0) {
        return line.start + d * t2;
    }

    return std::nullopt;
}

// Пересечение двух кругов
inline std::optional<Point2D> GetIntersectPoint(const Circle &circle1, const Circle &circle2) {
    double d = circle1.center_p.DistanceTo(circle2.center_p);

    if (d > circle1.radius + circle2.radius) {
        return std::nullopt;  // Круги не пересекаются
    }

    if (d < std::abs(circle1.radius - circle2.radius)) {
        return std::nullopt;  // Один круг внутри другого
    }

    if (d < 1e-10 && std::abs(circle1.radius - circle2.radius) < 1e-10) {
        return std::nullopt;  // Круги совпадают
    }

    double a = (circle1.radius * circle1.radius - circle2.radius * circle2.radius + d * d) / (2 * d);
    double h = std::sqrt(circle1.radius * circle1.radius - a * a);

    Point2D direction = (circle2.center_p - circle1.center_p) / d;
    Point2D p = circle1.center_p + direction * a;

    Point2D perpendicular{-direction.y, direction.x};
    return p + perpendicular * h;
}

inline std::optional<Point2D> GetIntersectPoint(const Shape &shape1, const Shape &shape2) { return std::nullopt; }

class IntersectionVisitor {
public:
    std::optional<Point2D> operator()(const Line &line1, const Line &line2) { return GetIntersectPoint(line1, line2); }
    std::optional<Point2D> operator()(const Line &line, const Circle &circle) {
        return GetIntersectPoint(line, circle);
    }
    std::optional<Point2D> operator()(const Circle &circle, const Line &line) {
        return GetIntersectPoint(line, circle);
    }
    std::optional<Point2D> operator()(const Circle &circle1, const Circle &circle2) {
        return GetIntersectPoint(circle1, circle2);
    }
    std::optional<Point2D> operator()(const auto &, const auto &) { throw std::logic_error("unsupported type"); }
};

}  // namespace geometry::intersections