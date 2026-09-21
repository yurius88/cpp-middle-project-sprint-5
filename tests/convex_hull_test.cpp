#include "convex_hull.hpp"
#include <gtest/gtest.h>

using namespace geometry;
using namespace geometry::convex_hull;

TEST(CrossProductTest, PositiveCross) {
    Point2D p1(1.0, 0.0);
    Point2D middle(0.0, 0.0);
    Point2D p2(0.0, 1.0);
    double result = CrossProduct(p1, middle, p2);
    EXPECT_GT(result, 0.0);
}

TEST(CrossProductTest, NegativeCross) {
    Point2D p1(0.0, 1.0);
    Point2D middle(0.0, 0.0);
    Point2D p2(1.0, 0.0);
    double result = CrossProduct(p1, middle, p2);
    EXPECT_LT(result, 0.0);
}

TEST(CrossProductTest, Collinear) {
    Point2D p1(1.0, 1.0);
    Point2D middle(0.0, 0.0);
    Point2D p2(2.0, 2.0);
    double result = CrossProduct(p1, middle, p2);
    EXPECT_NEAR(result, 0.0, 1e-10);
}

TEST(GrahamScanTest, LessThanThreePoints) {
    std::vector<Point2D> points = {{0.0, 0.0}, {1.0, 1.0}};
    auto result = GrahamScan(points);
    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), "At least three points are required for convex hull.");
}

TEST(GrahamScanTest, Triangle) {
    std::vector<Point2D> points = {{0.0, 0.0}, {4.0, 0.0}, {2.0, 3.0}};
    auto result = GrahamScan(points);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 3);
}

TEST(GrahamScanTest, Square) {
    std::vector<Point2D> points = {{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}};
    auto result = GrahamScan(points);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 4);
}

TEST(GrahamScanTest, PointsWithInteriorPoint) {
    std::vector<Point2D> points = {{0.0, 0.0}, {4.0, 0.0}, {4.0, 4.0}, {0.0, 4.0}, {2.0, 2.0}};
    auto result = GrahamScan(points);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 4);
}

TEST(GrahamScanTest, PentagonWithInteriorPoints) {
    std::vector<Point2D> points = {{0.0, 0.0}, {5.0, 0.0}, {6.0, 3.0}, {2.5, 5.0}, {-1.0, 3.0}, {2.5, 2.5}, {3.0, 1.0}};
    auto result = GrahamScan(points);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 5);
}

TEST(GrahamScanTest, CircularPoints) {
    std::vector<Point2D> points;
    for (int i = 0; i < 8; ++i) {
        double angle = 2 * 3.14159265359 * i / 8;
        points.push_back({5.0 * std::cos(angle), 5.0 * std::sin(angle)});
    }
    auto result = GrahamScan(points);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 8);
}

TEST(GrahamScanTest, ThreePointsExact) {
    std::vector<Point2D> points = {{0.0, 0.0}, {1.0, 0.0}, {0.5, 1.0}};
    auto result = GrahamScan(points);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 3);
}

TEST(StackForGrahamScanTest, PushAndPop) {
    StackForGrahamScan stack;
    stack.Push({0.0, 0.0});
    stack.Push({1.0, 1.0});
    EXPECT_EQ(stack.Size(), 2);
    stack.Pop();
    EXPECT_EQ(stack.Size(), 1);
}

TEST(StackForGrahamScanTest, TopAndNextToTop) {
    StackForGrahamScan stack;
    stack.Push({0.0, 0.0});
    stack.Push({1.0, 1.0});
    stack.Push({2.0, 2.0});
    Point2D top = stack.Top();
    Point2D next = stack.NextToTop();
    EXPECT_EQ(top.x, 2.0);
    EXPECT_EQ(top.y, 2.0);
    EXPECT_EQ(next.x, 1.0);
    EXPECT_EQ(next.y, 1.0);
}

TEST(StackForGrahamScanTest, Extract) {
    StackForGrahamScan stack;
    stack.Push({0.0, 0.0});
    stack.Push({1.0, 1.0});
    stack.Push({2.0, 2.0});
    auto points = stack.Extract();
    EXPECT_EQ(points.size(), 3);
    EXPECT_EQ(points[0].x, 0.0);
    EXPECT_EQ(points[1].x, 1.0);
    EXPECT_EQ(points[2].x, 2.0);
}
