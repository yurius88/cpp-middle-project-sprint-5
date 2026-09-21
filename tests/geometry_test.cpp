#include <gtest/gtest.h>
#include "geometry.hpp"

using namespace geometry;

TEST(Point2DTest, DefaultConstructor) {
    Point2D p;
    EXPECT_EQ(p.x, 0.0);
    EXPECT_EQ(p.y, 0.0);
}

TEST(Point2DTest, ParametrizedConstructor) {
    Point2D p(3.0, 4.0);
    EXPECT_EQ(p.x, 3.0);
    EXPECT_EQ(p.y, 4.0);
}

TEST(Point2DTest, EqualityOperator) {
    Point2D p1(1.0, 2.0);
    Point2D p2(1.0, 2.0);
    Point2D p3(2.0, 1.0);
    EXPECT_TRUE(p1 == p2);
    EXPECT_FALSE(p1 == p3);
}

TEST(Point2DTest, LessThanOperator) {
    Point2D p1(1.0, 2.0);
    Point2D p2(3.0, 4.0);
    EXPECT_TRUE(p1 < p2);
    EXPECT_FALSE(p2 < p1);
}

TEST(Point2DTest, Addition) {
    Point2D p1(1.0, 2.0);
    Point2D p2(3.0, 4.0);
    Point2D result = p1 + p2;
    EXPECT_EQ(result.x, 4.0);
    EXPECT_EQ(result.y, 6.0);
}

TEST(Point2DTest, Subtraction) {
    Point2D p1(5.0, 7.0);
    Point2D p2(2.0, 3.0);
    Point2D result = p1 - p2;
    EXPECT_EQ(result.x, 3.0);
    EXPECT_EQ(result.y, 4.0);
}

TEST(Point2DTest, MultiplicationByScalar) {
    Point2D p(2.0, 3.0);
    Point2D result = p * 2.0;
    EXPECT_EQ(result.x, 4.0);
    EXPECT_EQ(result.y, 6.0);
}

TEST(Point2DTest, DivisionByScalar) {
    Point2D p(6.0, 9.0);
    Point2D result = p / 3.0;
    EXPECT_EQ(result.x, 2.0);
    EXPECT_EQ(result.y, 3.0);
}

TEST(Point2DTest, DotProduct) {
    Point2D p1(2.0, 3.0);
    Point2D p2(4.0, 5.0);
    double result = p1.Dot(p2);
    EXPECT_NEAR(result, 23.0, 1e-10);
}

TEST(Point2DTest, CrossProduct) {
    Point2D p1(2.0, 3.0);
    Point2D p2(4.0, 5.0);
    double result = p1.Cross(p2);
    EXPECT_NEAR(result, -2.0, 1e-10);
}

TEST(Point2DTest, Length) {
    Point2D p(3.0, 4.0);
    EXPECT_NEAR(p.Length(), 5.0, 1e-10);
}

TEST(Point2DTest, DistanceTo) {
    Point2D p1(0.0, 0.0);
    Point2D p2(3.0, 4.0);
    EXPECT_NEAR(p1.DistanceTo(p2), 5.0, 1e-10);
}

TEST(Point2DTest, Normalize) {
    Point2D p(3.0, 4.0);
    Point2D normalized = p.Normalize();
    EXPECT_NEAR(normalized.x, 0.6, 1e-10);
    EXPECT_NEAR(normalized.y, 0.8, 1e-10);
    EXPECT_NEAR(normalized.Length(), 1.0, 1e-10);
}

TEST(BoundingBoxTest, Constructor) {
    BoundingBox bb(0.0, 0.0, 10.0, 10.0);
    EXPECT_EQ(bb.min_x, 0.0);
    EXPECT_EQ(bb.min_y, 0.0);
    EXPECT_EQ(bb.max_x, 10.0);
    EXPECT_EQ(bb.max_y, 10.0);
}

TEST(BoundingBoxTest, Overlaps) {
    BoundingBox bb1(0.0, 0.0, 5.0, 5.0);
    BoundingBox bb2(3.0, 3.0, 8.0, 8.0);
    BoundingBox bb3(10.0, 10.0, 15.0, 15.0);
    EXPECT_TRUE(bb1.Overlaps(bb2));
    EXPECT_FALSE(bb1.Overlaps(bb3));
}

TEST(BoundingBoxTest, WidthAndHeight) {
    BoundingBox bb(0.0, 0.0, 10.0, 5.0);
    EXPECT_EQ(bb.Width(), 10.0);
    EXPECT_EQ(bb.Height(), 5.0);
}

TEST(BoundingBoxTest, Center) {
    BoundingBox bb(0.0, 0.0, 10.0, 10.0);
    Point2D center = bb.Center();
    EXPECT_EQ(center.x, 5.0);
    EXPECT_EQ(center.y, 5.0);
}

TEST(LineTest, Constructor) {
    Line line({0.0, 0.0}, {3.0, 4.0});
    EXPECT_EQ(line.start.x, 0.0);
    EXPECT_EQ(line.start.y, 0.0);
    EXPECT_EQ(line.end.x, 3.0);
    EXPECT_EQ(line.end.y, 4.0);
}

TEST(LineTest, Length) {
    Line line({0.0, 0.0}, {3.0, 4.0});
    EXPECT_NEAR(line.Length(), 5.0, 1e-10);
}

TEST(LineTest, BoundBox) {
    Line line({1.0, 2.0}, {5.0, 8.0});
    BoundingBox bb = line.BoundBox();
    EXPECT_EQ(bb.min_x, 1.0);
    EXPECT_EQ(bb.min_y, 2.0);
    EXPECT_EQ(bb.max_x, 5.0);
    EXPECT_EQ(bb.max_y, 8.0);
}

TEST(TriangleTest, Constructor) {
    Triangle tri({0.0, 0.0}, {4.0, 0.0}, {2.0, 3.0});
    EXPECT_EQ(tri.a.x, 0.0);
    EXPECT_EQ(tri.b.x, 4.0);
    EXPECT_EQ(tri.c.x, 2.0);
}

TEST(TriangleTest, Area) {
    Triangle tri({0.0, 0.0}, {4.0, 0.0}, {0.0, 3.0});
    EXPECT_NEAR(tri.Area(), 6.0, 1e-10);
}

TEST(TriangleTest, BoundBox) {
    Triangle tri({0.0, 0.0}, {4.0, 1.0}, {2.0, 5.0});
    BoundingBox bb = tri.BoundBox();
    EXPECT_EQ(bb.min_x, 0.0);
    EXPECT_EQ(bb.min_y, 0.0);
    EXPECT_EQ(bb.max_x, 4.0);
    EXPECT_EQ(bb.max_y, 5.0);
}

TEST(RectangleTest, Constructor) {
    Rectangle rect({0.0, 0.0}, 10.0, 5.0);
    EXPECT_EQ(rect.bottom_left.x, 0.0);
    EXPECT_EQ(rect.width, 10.0);
    EXPECT_EQ(rect.height, 5.0);
}

TEST(RectangleTest, TopRight) {
    Rectangle rect({1.0, 2.0}, 10.0, 5.0);
    Point2D tr = rect.TopRight();
    EXPECT_EQ(tr.x, 11.0);
    EXPECT_EQ(tr.y, 7.0);
}

TEST(RectangleTest, BoundBox) {
    Rectangle rect({1.0, 2.0}, 10.0, 5.0);
    BoundingBox bb = rect.BoundBox();
    EXPECT_EQ(bb.min_x, 1.0);
    EXPECT_EQ(bb.min_y, 2.0);
    EXPECT_EQ(bb.max_x, 11.0);
    EXPECT_EQ(bb.max_y, 7.0);
}

TEST(CircleTest, Constructor) {
    Circle circle({5.0, 5.0}, 3.0);
    EXPECT_EQ(circle.center_p.x, 5.0);
    EXPECT_EQ(circle.center_p.y, 5.0);
    EXPECT_EQ(circle.radius, 3.0);
}

TEST(CircleTest, BoundBox) {
    Circle circle({5.0, 5.0}, 3.0);
    BoundingBox bb = circle.BoundBox();
    EXPECT_EQ(bb.min_x, 2.0);
    EXPECT_EQ(bb.min_y, 2.0);
    EXPECT_EQ(bb.max_x, 8.0);
    EXPECT_EQ(bb.max_y, 8.0);
}

TEST(RegularPolygonTest, Constructor) {
    RegularPolygon poly({0.0, 0.0}, 5.0, 6);
    EXPECT_EQ(poly.center_p.x, 0.0);
    EXPECT_EQ(poly.radius, 5.0);
    EXPECT_EQ(poly.sides, 6);
}

TEST(RegularPolygonTest, Vertices) {
    RegularPolygon poly({0.0, 0.0}, 1.0, 4);
    auto vertices = poly.Vertices();
    EXPECT_EQ(vertices.size(), 4);
}

TEST(PolygonTest, Constructor) {
    std::vector<Point2D> points = {{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}};
    Polygon poly(points);
    EXPECT_EQ(poly.Vertices().size(), 4);
}

TEST(PolygonTest, BoundBox) {
    std::vector<Point2D> points = {{0.0, 0.0}, {5.0, 0.0}, {5.0, 3.0}, {0.0, 3.0}};
    Polygon poly(points);
    BoundingBox bb = poly.BoundBox();
    EXPECT_EQ(bb.min_x, 0.0);
    EXPECT_EQ(bb.min_y, 0.0);
    EXPECT_EQ(bb.max_x, 5.0);
    EXPECT_EQ(bb.max_y, 3.0);
}
