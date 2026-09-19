#include "shape_utils.hpp"
#include <gtest/gtest.h>

using namespace geometry;
using namespace geometry::utils;

TEST(ParseShapesTest, ParseCircle) {
    auto shapes = ParseShapes("circle 0 0 5");
    ASSERT_EQ(shapes.size(), 1);
    ASSERT_TRUE(std::holds_alternative<Circle>(shapes[0]));
    Circle circle = std::get<Circle>(shapes[0]);
    EXPECT_EQ(circle.center_p.x, 0.0);
    EXPECT_EQ(circle.center_p.y, 0.0);
    EXPECT_EQ(circle.radius, 5.0);
}

TEST(ParseShapesTest, ParseLine) {
    auto shapes = ParseShapes("line 0 0 4 3");
    ASSERT_EQ(shapes.size(), 1);
    ASSERT_TRUE(std::holds_alternative<Line>(shapes[0]));
    Line line = std::get<Line>(shapes[0]);
    EXPECT_EQ(line.start.x, 0.0);
    EXPECT_EQ(line.start.y, 0.0);
    EXPECT_EQ(line.end.x, 4.0);
    EXPECT_EQ(line.end.y, 3.0);
}

TEST(ParseShapesTest, ParseTriangle) {
    auto shapes = ParseShapes("triangle 0 0 4 0 2 3");
    ASSERT_EQ(shapes.size(), 1);
    ASSERT_TRUE(std::holds_alternative<Triangle>(shapes[0]));
    Triangle tri = std::get<Triangle>(shapes[0]);
    EXPECT_EQ(tri.a.x, 0.0);
    EXPECT_EQ(tri.b.x, 4.0);
    EXPECT_EQ(tri.c.x, 2.0);
}

TEST(ParseShapesTest, ParseRectangle) {
    auto shapes = ParseShapes("rectangle 0 0 10 5");
    ASSERT_EQ(shapes.size(), 1);
    ASSERT_TRUE(std::holds_alternative<Rectangle>(shapes[0]));
    Rectangle rect = std::get<Rectangle>(shapes[0]);
    EXPECT_EQ(rect.bottom_left.x, 0.0);
    EXPECT_EQ(rect.bottom_left.y, 0.0);
    EXPECT_EQ(rect.width, 10.0);
    EXPECT_EQ(rect.height, 5.0);
}

TEST(ParseShapesTest, ParsePolygon) {
    auto shapes = ParseShapes("polygon 0 0 5 6");
    ASSERT_EQ(shapes.size(), 1);
    ASSERT_TRUE(std::holds_alternative<RegularPolygon>(shapes[0]));
    RegularPolygon poly = std::get<RegularPolygon>(shapes[0]);
    EXPECT_EQ(poly.center_p.x, 0.0);
    EXPECT_EQ(poly.center_p.y, 0.0);
    EXPECT_EQ(poly.radius, 5.0);
    EXPECT_EQ(poly.sides, 6);
}

TEST(ParseShapesTest, ParseMultipleShapes) {
    auto shapes = ParseShapes("circle 0 0 5; line 0 0 4 3; rectangle 1 1 3 2");
    ASSERT_EQ(shapes.size(), 3);
    EXPECT_TRUE(std::holds_alternative<Circle>(shapes[0]));
    EXPECT_TRUE(std::holds_alternative<Line>(shapes[1]));
    EXPECT_TRUE(std::holds_alternative<Rectangle>(shapes[2]));
}

TEST(ParseShapesTest, ParseInvalidShape) {
    auto shapes = ParseShapes("invalid 0 0 5");
    EXPECT_EQ(shapes.size(), 0);
}

TEST(ParseShapesTest, ParseInvalidCircleNegativeRadius) {
    auto shapes = ParseShapes("circle 0 0 -5");
    EXPECT_EQ(shapes.size(), 0);
}

TEST(ParseShapesTest, ParseInvalidRectangleNegativeWidth) {
    auto shapes = ParseShapes("rectangle 0 0 -10 5");
    EXPECT_EQ(shapes.size(), 0);
}

TEST(ParseShapesTest, ParseInvalidPolygonLessThan3Sides) {
    auto shapes = ParseShapes("polygon 0 0 5 2");
    EXPECT_EQ(shapes.size(), 0);
}

TEST(ParseShapesTest, ParseInvalidPolygonNonIntegerSides) {
    auto shapes = ParseShapes("polygon 0 0 5 6.5");
    EXPECT_EQ(shapes.size(), 0);
}

TEST(ParseShapesTest, ParseEmptyString) {
    auto shapes = ParseShapes("");
    EXPECT_EQ(shapes.size(), 0);
}

TEST(ParseShapesTest, ParseWithExtraWhitespace) {
    auto shapes = ParseShapes("  circle   0   0   5  ;  line 0 0 4 3  ");
    ASSERT_EQ(shapes.size(), 2);
    EXPECT_TRUE(std::holds_alternative<Circle>(shapes[0]));
    EXPECT_TRUE(std::holds_alternative<Line>(shapes[1]));
}

TEST(FindAllCollisionsTest, NoCollisions) {
    auto shapes = ParseShapes("circle 0 0 1; circle 10 10 1");
    auto collisions = FindAllCollisions(shapes);
    EXPECT_EQ(collisions.size(), 0);
}

TEST(FindAllCollisionsTest, TwoShapesColliding) {
    auto shapes = ParseShapes("circle 0 0 5; circle 5 0 5");
    auto collisions = FindAllCollisions(shapes);
    EXPECT_EQ(collisions.size(), 1);
}

TEST(FindAllCollisionsTest, MultipleCollisions) {
    auto shapes = ParseShapes("circle 0 0 5; circle 5 0 5; circle 2.5 0 5");
    auto collisions = FindAllCollisions(shapes);
    EXPECT_GE(collisions.size(), 1);
}

TEST(FindAllCollisionsTest, SingleShape) {
    auto shapes = ParseShapes("circle 0 0 5");
    auto collisions = FindAllCollisions(shapes);
    EXPECT_EQ(collisions.size(), 0);
}

TEST(FindAllCollisionsTest, EmptyShapeList) {
    std::vector<Shape> shapes;
    auto collisions = FindAllCollisions(shapes);
    EXPECT_EQ(collisions.size(), 0);
}

TEST(FindHighestShapeTest, SingleShape) {
    auto shapes = ParseShapes("circle 0 5 3");
    auto result = FindHighestShape(shapes);
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(*result, 8.0, 1e-10);
}

TEST(FindHighestShapeTest, MultipleShapes) {
    auto shapes = ParseShapes("circle 0 5 3; rectangle 0 0 10 2; circle 0 10 1");
    auto result = FindHighestShape(shapes);
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(*result, 11.0, 1e-10);
}

TEST(FindHighestShapeTest, EmptyShapeList) {
    std::vector<Shape> shapes;
    auto result = FindHighestShape(shapes);
    EXPECT_FALSE(result.has_value());
}
