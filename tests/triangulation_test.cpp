#include <gtest/gtest.h>
#include "triangulation.hpp"

using namespace geometry;
using namespace geometry::triangulation;

TEST(DelaunayTriangleTest, Constructor) {
    DelaunayTriangle tri({0.0, 0.0}, {4.0, 0.0}, {2.0, 3.0});
    EXPECT_EQ(tri.a.x, 0.0);
    EXPECT_EQ(tri.b.x, 4.0);
    EXPECT_EQ(tri.c.x, 2.0);
}

TEST(DelaunayTriangleTest, Circumcenter) {
    DelaunayTriangle tri({0.0, 0.0}, {4.0, 0.0}, {2.0, 4.0});
    Point2D center = tri.Circumcenter();
    EXPECT_NEAR(center.x, 2.0, 1e-9);
    EXPECT_GT(center.y, 0.0);
}

TEST(DelaunayTriangleTest, Circumradius) {
    DelaunayTriangle tri({0.0, 0.0}, {4.0, 0.0}, {2.0, 4.0});
    double radius = tri.Circumradius();
    EXPECT_GT(radius, 0.0);
}

TEST(DelaunayTriangleTest, ContainsPointInside) {
    DelaunayTriangle tri({0.0, 0.0}, {4.0, 0.0}, {2.0, 4.0});
    Point2D center = tri.Circumcenter();
    EXPECT_TRUE(tri.ContainsPoint(center));
}

TEST(DelaunayTriangleTest, ContainsPointOutside) {
    DelaunayTriangle tri({0.0, 0.0}, {4.0, 0.0}, {2.0, 4.0});
    Point2D far_point(100.0, 100.0);
    EXPECT_FALSE(tri.ContainsPoint(far_point));
}

TEST(DelaunayTriangleTest, SharesEdgeTrue) {
    DelaunayTriangle tri1({0.0, 0.0}, {4.0, 0.0}, {2.0, 3.0});
    DelaunayTriangle tri2({0.0, 0.0}, {4.0, 0.0}, {2.0, -3.0});
    EXPECT_TRUE(tri1.SharesEdge(tri2));
}

TEST(DelaunayTriangleTest, SharesEdgeFalse) {
    DelaunayTriangle tri1({0.0, 0.0}, {4.0, 0.0}, {2.0, 3.0});
    DelaunayTriangle tri2({10.0, 10.0}, {14.0, 10.0}, {12.0, 13.0});
    EXPECT_FALSE(tri1.SharesEdge(tri2));
}

TEST(DelaunayTriangleTest, Vertices) {
    DelaunayTriangle tri({0.0, 0.0}, {4.0, 0.0}, {2.0, 3.0});
    auto vertices = tri.vertices();
    ASSERT_EQ(vertices.size(), 3);
    EXPECT_EQ(vertices[0].x, 0.0);
    EXPECT_EQ(vertices[1].x, 4.0);
    EXPECT_EQ(vertices[2].x, 2.0);
}

TEST(EdgeTest, Constructor) {
    Edge edge({0.0, 0.0}, {4.0, 3.0});
    EXPECT_EQ(edge.p1.x, 0.0);
    EXPECT_EQ(edge.p2.x, 4.0);
}

TEST(EdgeTest, ConstructorSortsPoints) {
    Edge edge({4.0, 3.0}, {0.0, 0.0});
    EXPECT_EQ(edge.p1.x, 0.0);
    EXPECT_EQ(edge.p2.x, 4.0);
}

TEST(EdgeTest, EqualityOperator) {
    Edge edge1({0.0, 0.0}, {4.0, 3.0});
    Edge edge2({0.0, 0.0}, {4.0, 3.0});
    EXPECT_TRUE(edge1 == edge2);
}

TEST(EdgeTest, LessThanOperator) {
    Edge edge1({0.0, 0.0}, {2.0, 0.0});
    Edge edge2({1.0, 0.0}, {3.0, 0.0});
    EXPECT_TRUE(edge1 < edge2);
}

TEST(DelaunayTriangulationTest, LessThanThreePoints) {
    std::vector<Point2D> points = {{0.0, 0.0}, {1.0, 1.0}};
    auto result = DelaunayTriangulation(points);
    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), "At least three points are required for triangulation.");
}

TEST(DelaunayTriangulationTest, ThreePoints) {
    std::vector<Point2D> points = {{0.0, 0.0}, {4.0, 0.0}, {2.0, 3.0}};
    auto result = DelaunayTriangulation(points);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 1);
}

TEST(DelaunayTriangulationTest, FourPointsSquare) {
    std::vector<Point2D> points = {{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}};
    auto result = DelaunayTriangulation(points);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 2);
}

TEST(DelaunayTriangulationTest, FivePointsWithCenter) {
    std::vector<Point2D> points = {
        {0.0, 0.0}, {4.0, 0.0}, {4.0, 4.0}, {0.0, 4.0}, {2.0, 2.0}
    };
    auto result = DelaunayTriangulation(points);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 4);
}

TEST(DelaunayTriangulationTest, SixPointsHexagon) {
    std::vector<Point2D> points;
    for (int i = 0; i < 6; ++i) {
        double angle = 2 * 3.14159265359 * i / 6;
        points.push_back({std::cos(angle), std::sin(angle)});
    }
    auto result = DelaunayTriangulation(points);
    ASSERT_TRUE(result.has_value());
    EXPECT_GE(result->size(), 4);
}

TEST(DelaunayTriangulationTest, RandomPoints) {
    std::vector<Point2D> points = {
        {0.0, 0.0}, {1.0, 2.0}, {3.0, 1.0}, {4.0, 4.0},
        {2.0, 3.0}, {5.0, 2.0}, {3.5, 3.5}
    };
    auto result = DelaunayTriangulation(points);
    ASSERT_TRUE(result.has_value());
    EXPECT_GE(result->size(), 1);
}

TEST(DelaunayTriangulationTest, CollinearPoints) {
    std::vector<Point2D> points = {{0.0, 0.0}, {1.0, 1.0}, {2.0, 2.0}, {3.0, 3.0}};
    auto result = DelaunayTriangulation(points);
    ASSERT_TRUE(result.has_value());
}

TEST(DelaunayTriangulationTest, VerifyNoSuperTriangleVertices) {
    std::vector<Point2D> points = {{0.0, 0.0}, {1.0, 0.0}, {0.5, 1.0}};
    auto result = DelaunayTriangulation(points);
    ASSERT_TRUE(result.has_value());

    for (const auto& tri : *result) {
        EXPECT_LE(std::abs(tri.a.x), 10.0);
        EXPECT_LE(std::abs(tri.a.y), 10.0);
        EXPECT_LE(std::abs(tri.b.x), 10.0);
        EXPECT_LE(std::abs(tri.b.y), 10.0);
        EXPECT_LE(std::abs(tri.c.x), 10.0);
        EXPECT_LE(std::abs(tri.c.y), 10.0);
    }
}

TEST(DelaunayTriangulationTest, LargePointSet) {
    std::vector<Point2D> points;
    for (int i = 0; i < 10; ++i) {
        double angle = 2 * 3.14159265359 * i / 10;
        points.push_back({5.0 * std::cos(angle), 5.0 * std::sin(angle)});
    }
    auto result = DelaunayTriangulation(points);
    ASSERT_TRUE(result.has_value());
    EXPECT_GE(result->size(), 8);
}
