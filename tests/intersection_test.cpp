#include "intersections.hpp"
#include <gtest/gtest.h>

using namespace geometry;
using namespace geometry::intersections;

TEST(IntersectionTest, LineLineIntersecting) {
    Line line1({0.0, 0.0}, {4.0, 4.0});
    Line line2({0.0, 4.0}, {4.0, 0.0});
    auto result = GetIntersectPoint(line1, line2);
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(result->x, 2.0, 1e-10);
    EXPECT_NEAR(result->y, 2.0, 1e-10);
}

TEST(IntersectionTest, LineLineParallel) {
    Line line1({0.0, 0.0}, {4.0, 0.0});
    Line line2({0.0, 1.0}, {4.0, 1.0});
    auto result = GetIntersectPoint(line1, line2);
    EXPECT_FALSE(result.has_value());
}

TEST(IntersectionTest, LineLineNotIntersecting) {
    Line line1({0.0, 0.0}, {1.0, 1.0});
    Line line2({2.0, 0.0}, {3.0, 1.0});
    auto result = GetIntersectPoint(line1, line2);
    EXPECT_FALSE(result.has_value());
}

TEST(IntersectionTest, LineLinePerpendicular) {
    Line line1({0.0, 0.0}, {4.0, 0.0});
    Line line2({2.0, -2.0}, {2.0, 2.0});
    auto result = GetIntersectPoint(line1, line2);
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(result->x, 2.0, 1e-10);
    EXPECT_NEAR(result->y, 0.0, 1e-10);
}

TEST(IntersectionTest, LineCircleIntersecting) {
    Line line({-5.0, 0.0}, {5.0, 0.0});
    Circle circle({0.0, 0.0}, 3.0);
    auto result = GetIntersectPoint(line, circle);
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(std::abs(result->x), 3.0, 1e-10);
    EXPECT_NEAR(result->y, 0.0, 1e-10);
}

TEST(IntersectionTest, LineCircleNotIntersecting) {
    Line line({0.0, 5.0}, {4.0, 5.0});
    Circle circle({0.0, 0.0}, 3.0);
    auto result = GetIntersectPoint(line, circle);
    EXPECT_FALSE(result.has_value());
}

TEST(IntersectionTest, LineCircleTangent) {
    Line line({-5.0, 3.0}, {5.0, 3.0});
    Circle circle({0.0, 0.0}, 3.0);
    auto result = GetIntersectPoint(line, circle);
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(result->x, 0.0, 1e-9);
    EXPECT_NEAR(result->y, 3.0, 1e-9);
}

TEST(IntersectionTest, LineCircleLineInside) {
    Line line({0.0, 0.0}, {6.0, 0.0});
    Circle circle({0.0, 0.0}, 5.0);
    auto result = GetIntersectPoint(line, circle);
    ASSERT_TRUE(result.has_value());
}

TEST(IntersectionTest, CircleCircleIntersecting) {
    Circle circle1({0.0, 0.0}, 3.0);
    Circle circle2({4.0, 0.0}, 3.0);
    auto result = GetIntersectPoint(circle1, circle2);
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(result->x, 2.0, 1e-9);
}

TEST(IntersectionTest, CircleCircleNotIntersecting) {
    Circle circle1({0.0, 0.0}, 2.0);
    Circle circle2({10.0, 0.0}, 2.0);
    auto result = GetIntersectPoint(circle1, circle2);
    EXPECT_FALSE(result.has_value());
}

TEST(IntersectionTest, CircleCircleOneInsideOther) {
    Circle circle1({0.0, 0.0}, 5.0);
    Circle circle2({1.0, 0.0}, 1.0);
    auto result = GetIntersectPoint(circle1, circle2);
    EXPECT_FALSE(result.has_value());
}

TEST(IntersectionTest, CircleCircleTangent) {
    Circle circle1({0.0, 0.0}, 3.0);
    Circle circle2({6.0, 0.0}, 3.0);
    auto result = GetIntersectPoint(circle1, circle2);
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(result->x, 3.0, 1e-9);
    EXPECT_NEAR(result->y, 0.0, 1e-9);
}

TEST(IntersectionTest, CircleCircleIdentical) {
    Circle circle1({0.0, 0.0}, 3.0);
    Circle circle2({0.0, 0.0}, 3.0);
    auto result = GetIntersectPoint(circle1, circle2);
    EXPECT_FALSE(result.has_value());
}

TEST(IntersectionVisitorTest, LineLineIntersecting) {
    Line line1({0.0, 0.0}, {4.0, 4.0});
    Line line2({0.0, 4.0}, {4.0, 0.0});
    IntersectionVisitor visitor;
    auto result = visitor(line1, line2);
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(result->x, 2.0, 1e-10);
    EXPECT_NEAR(result->y, 2.0, 1e-10);
}

TEST(IntersectionVisitorTest, LineCircleIntersecting) {
    Line line({-5.0, 0.0}, {5.0, 0.0});
    Circle circle({0.0, 0.0}, 3.0);
    IntersectionVisitor visitor;
    auto result = visitor(line, circle);
    ASSERT_TRUE(result.has_value());
}

TEST(IntersectionVisitorTest, CircleLineIntersecting) {
    Circle circle({0.0, 0.0}, 3.0);
    Line line({-5.0, 0.0}, {5.0, 0.0});
    IntersectionVisitor visitor;
    auto result = visitor(circle, line);
    ASSERT_TRUE(result.has_value());
}

TEST(IntersectionVisitorTest, CircleCircleIntersecting) {
    Circle circle1({0.0, 0.0}, 3.0);
    Circle circle2({4.0, 0.0}, 3.0);
    IntersectionVisitor visitor;
    auto result = visitor(circle1, circle2);
    ASSERT_TRUE(result.has_value());
}

TEST(IntersectionVisitorTest, UnsupportedTypes) {
    Triangle tri({0.0, 0.0}, {4.0, 0.0}, {2.0, 3.0});
    Rectangle rect({0.0, 0.0}, 4.0, 3.0);
    IntersectionVisitor visitor;
    EXPECT_THROW(visitor(tri, rect), std::logic_error);
}

TEST(IntersectionVisitorTest, TriangleCircleUnsupported) {
    Triangle tri({0.0, 0.0}, {4.0, 0.0}, {2.0, 3.0});
    Circle circle({0.0, 0.0}, 3.0);
    IntersectionVisitor visitor;
    EXPECT_THROW(visitor(tri, circle), std::logic_error);
}

TEST(IntersectionVisitorTest, RectangleLineUnsupported) {
    Rectangle rect({0.0, 0.0}, 4.0, 3.0);
    Line line({0.0, 0.0}, {4.0, 4.0});
    IntersectionVisitor visitor;
    EXPECT_THROW(visitor(rect, line), std::logic_error);
}
