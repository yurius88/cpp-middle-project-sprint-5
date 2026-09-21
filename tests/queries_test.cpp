#include <gtest/gtest.h>
#include "queries.hpp"

using namespace geometry;
using namespace geometry::queries;

TEST(DistanceVisitorTest, DistanceToLine) {
    Line line({0.0, 0.0}, {4.0, 0.0});
    Point2D point(2.0, 3.0);
    DistanceVisitor visitor(point);
    double distance = visitor(line);
    EXPECT_NEAR(distance, 3.0, 1e-10);
}

TEST(DistanceVisitorTest, DistanceToLineEndpoint) {
    Line line({0.0, 0.0}, {4.0, 0.0});
    Point2D point(5.0, 0.0);
    DistanceVisitor visitor(point);
    double distance = visitor(line);
    EXPECT_NEAR(distance, 1.0, 1e-10);
}

TEST(DistanceVisitorTest, DistanceToTriangle) {
    Triangle tri({0.0, 0.0}, {4.0, 0.0}, {2.0, 3.0});
    Point2D point(2.0, 1.0);
    DistanceVisitor visitor(point);
    double distance = visitor(tri);
    EXPECT_GE(distance, 0.0);
}

TEST(DistanceVisitorTest, DistanceToRectangle) {
    Rectangle rect({0.0, 0.0}, 4.0, 3.0);
    Point2D point(5.0, 1.5);
    DistanceVisitor visitor(point);
    double distance = visitor(rect);
    EXPECT_NEAR(distance, 1.0, 1e-10);
}

TEST(DistanceVisitorTest, DistanceToCircleOutside) {
    Circle circle({0.0, 0.0}, 5.0);
    Point2D point(10.0, 0.0);
    DistanceVisitor visitor(point);
    double distance = visitor(circle);
    EXPECT_NEAR(distance, 5.0, 1e-10);
}

TEST(DistanceVisitorTest, DistanceToCircleInside) {
    Circle circle({0.0, 0.0}, 5.0);
    Point2D point(2.0, 0.0);
    DistanceVisitor visitor(point);
    double distance = visitor(circle);
    EXPECT_NEAR(distance, 0.0, 1e-10);
}

TEST(DistanceVisitorTest, DistanceToRegularPolygon) {
    RegularPolygon poly({0.0, 0.0}, 5.0, 6);
    Point2D point(10.0, 0.0);
    DistanceVisitor visitor(point);
    double distance = visitor(poly);
    EXPECT_GE(distance, 0.0);
}

TEST(PointInShapeVisitorTest, PointOnLine) {
    Line line({0.0, 0.0}, {4.0, 0.0});
    Point2D point(2.0, 0.0);
    PointInShapeVisitor visitor(point);
    EXPECT_TRUE(visitor(line));
}

TEST(PointInShapeVisitorTest, PointNotOnLine) {
    Line line({0.0, 0.0}, {4.0, 0.0});
    Point2D point(2.0, 1.0);
    PointInShapeVisitor visitor(point);
    EXPECT_FALSE(visitor(line));
}

TEST(PointInShapeVisitorTest, PointInTriangle) {
    Triangle tri({0.0, 0.0}, {4.0, 0.0}, {2.0, 3.0});
    Point2D point(2.0, 1.0);
    PointInShapeVisitor visitor(point);
    EXPECT_TRUE(visitor(tri));
}

TEST(PointInShapeVisitorTest, PointOutsideTriangle) {
    Triangle tri({0.0, 0.0}, {4.0, 0.0}, {2.0, 3.0});
    Point2D point(5.0, 5.0);
    PointInShapeVisitor visitor(point);
    EXPECT_FALSE(visitor(tri));
}

TEST(PointInShapeVisitorTest, PointInRectangle) {
    Rectangle rect({0.0, 0.0}, 4.0, 3.0);
    Point2D point(2.0, 1.5);
    PointInShapeVisitor visitor(point);
    EXPECT_TRUE(visitor(rect));
}

TEST(PointInShapeVisitorTest, PointOutsideRectangle) {
    Rectangle rect({0.0, 0.0}, 4.0, 3.0);
    Point2D point(5.0, 5.0);
    PointInShapeVisitor visitor(point);
    EXPECT_FALSE(visitor(rect));
}

TEST(PointInShapeVisitorTest, PointInCircle) {
    Circle circle({0.0, 0.0}, 5.0);
    Point2D point(3.0, 0.0);
    PointInShapeVisitor visitor(point);
    EXPECT_TRUE(visitor(circle));
}

TEST(PointInShapeVisitorTest, PointOutsideCircle) {
    Circle circle({0.0, 0.0}, 5.0);
    Point2D point(10.0, 0.0);
    PointInShapeVisitor visitor(point);
    EXPECT_FALSE(visitor(circle));
}

TEST(PointInShapeVisitorTest, PointInRegularPolygon) {
    RegularPolygon poly({0.0, 0.0}, 5.0, 6);
    Point2D point(0.0, 0.0);
    PointInShapeVisitor visitor(point);
    EXPECT_TRUE(visitor(poly));
}

TEST(ShapeToShapeDistanceVisitorTest, CircleToCircleSeparated) {
    Circle c1({0.0, 0.0}, 3.0);
    Circle c2({10.0, 0.0}, 2.0);
    ShapeToShapeDistanceVisitor visitor;
    auto result = visitor(c1, c2);
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(*result, 5.0, 1e-10);
}

TEST(ShapeToShapeDistanceVisitorTest, CircleToCircleOverlapping) {
    Circle c1({0.0, 0.0}, 3.0);
    Circle c2({4.0, 0.0}, 2.0);
    ShapeToShapeDistanceVisitor visitor;
    auto result = visitor(c1, c2);
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(*result, 0.0, 1e-10);
}

TEST(ShapeToShapeDistanceVisitorTest, LineToLine) {
    Line l1({0.0, 0.0}, {2.0, 0.0});
    Line l2({0.0, 3.0}, {2.0, 3.0});
    ShapeToShapeDistanceVisitor visitor;
    auto result = visitor(l1, l2);
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(*result, 3.0, 1e-10);
}

TEST(ShapeToShapeDistanceVisitorTest, UnsupportedCombination) {
    Circle c({0.0, 0.0}, 3.0);
    Triangle tri({0.0, 0.0}, {4.0, 0.0}, {2.0, 3.0});
    ShapeToShapeDistanceVisitor visitor;
    auto result = visitor(c, tri);
    EXPECT_FALSE(result.has_value());
}

TEST(QueriesHelperTest, DistanceToPoint) {
    Circle circle({0.0, 0.0}, 5.0);
    Point2D point(10.0, 0.0);
    Shape shape = circle;
    double distance = DistanceToPoint(shape, point);
    EXPECT_NEAR(distance, 5.0, 1e-10);
}

TEST(QueriesHelperTest, GetBoundBox) {
    Rectangle rect({1.0, 2.0}, 10.0, 5.0);
    Shape shape = rect;
    BoundingBox bb = GetBoundBox(shape);
    EXPECT_EQ(bb.min_x, 1.0);
    EXPECT_EQ(bb.min_y, 2.0);
    EXPECT_EQ(bb.max_x, 11.0);
    EXPECT_EQ(bb.max_y, 7.0);
}

TEST(QueriesHelperTest, GetHeight) {
    Circle circle({5.0, 5.0}, 3.0);
    Shape shape = circle;
    double height = GetHeight(shape);
    EXPECT_NEAR(height, 8.0, 1e-10);
}

TEST(QueriesHelperTest, BoundingBoxesOverlap) {
    Rectangle rect1({0.0, 0.0}, 5.0, 5.0);
    Rectangle rect2({3.0, 3.0}, 5.0, 5.0);
    Shape shape1 = rect1;
    Shape shape2 = rect2;
    EXPECT_TRUE(BoundingBoxesOverlap(shape1, shape2));
}

TEST(QueriesHelperTest, BoundingBoxesDoNotOverlap) {
    Rectangle rect1({0.0, 0.0}, 2.0, 2.0);
    Rectangle rect2({10.0, 10.0}, 2.0, 2.0);
    Shape shape1 = rect1;
    Shape shape2 = rect2;
    EXPECT_FALSE(BoundingBoxesOverlap(shape1, shape2));
}

TEST(QueriesHelperTest, DistanceBetweenShapes) {
    Circle c1({0.0, 0.0}, 3.0);
    Circle c2({10.0, 0.0}, 2.0);
    Shape shape1 = c1;
    Shape shape2 = c2;
    auto result = DistanceBetweenShapes(shape1, shape2);
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(*result, 5.0, 1e-10);
}
