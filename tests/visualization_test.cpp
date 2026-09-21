#include <gtest/gtest.h>
#include "visualization.hpp"
#include "geometry.hpp"
#include "triangulation.hpp"

using namespace geometry;

// Тесты визуализации закомментированы, так как функция Draw() открывает графическое окно
// и требует взаимодействия пользователя, что не подходит для автоматических тестов.
// Раскомментируйте эти тесты только для ручного тестирования.

/*
TEST(VisualizationTest, DrawShapesNoThrow) {
    std::vector<Shape> shapes;
    shapes.push_back(Circle({0.0, 0.0}, 5.0));
    shapes.push_back(Line({0.0, 0.0}, {4.0, 3.0}));
    shapes.push_back(Rectangle({1.0, 1.0}, 3.0, 2.0));

    EXPECT_NO_THROW(geometry::visualization::Draw(shapes));
}

TEST(VisualizationTest, DrawTriangulationNoThrow) {
    std::vector<Point2D> points = {{0.0, 0.0}, {4.0, 0.0}, {2.0, 3.0}};
    auto result = geometry::triangulation::DelaunayTriangulation(points);
    ASSERT_TRUE(result.has_value());

    EXPECT_NO_THROW(geometry::visualization::Draw(*result));
}
*/

// Простой тест, чтобы файл не был пустым
TEST(VisualizationTest, PlaceholderTest) {
    EXPECT_TRUE(true);
}
