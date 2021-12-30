#include <gtest/gtest.h>
#include <geometry/bulge.h>

static const Eigen::Vector2d point1(1.2, 3.4);
static const Eigen::Vector2d point2(4.5, 6.7);
static const Eigen::Vector2d point3(7.8, 9.1);
static const Eigen::Vector2d point4(11.0, 12.0);

static const geometry::Bulge bulge1(point1, point2, 0.0f);
static const geometry::Bulge bulge2(point2, point3, 1.0f);
static const geometry::Bulge bulge3(point3, point4, 0.0f);

TEST(BulgeTest, Length)
{
	EXPECT_FLOAT_EQ(bulge1.length(), (point1 - point2).norm());
	EXPECT_FLOAT_EQ(bulge2.length(), (point2 - point3).norm() / 2.0f * M_PI);
	EXPECT_FLOAT_EQ(bulge3.length(), (point3 - point4).norm());
}

TEST(BulgeTest, IsLine)
{
	ASSERT_TRUE(bulge1.isLine());
	ASSERT_FALSE(bulge2.isLine());
	ASSERT_TRUE(bulge3.isLine());
}

TEST(BulgeTest, IsArc)
{
	ASSERT_FALSE(bulge1.isArc());
	ASSERT_TRUE(bulge2.isArc());
	ASSERT_FALSE(bulge3.isArc());
}

TEST(BulgeTest, LinifyBecomeIsLine)
{
	geometry::Bulge bulge(bulge2);
	bulge.linify();

	ASSERT_TRUE(bulge.isLine());
}
