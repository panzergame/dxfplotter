#include <gtest/gtest.h>
#include <geometry/bulge.h>

constexpr QVector2D point1(1.2, 3.4);
constexpr QVector2D point2(4.5, 6.7);
constexpr QVector2D point3(7.8, 9.1);
constexpr QVector2D point4(11.0, 12.0);

static const geometry::Bulge bulge1(point1, point2, 0.0f);
static const geometry::Bulge bulge2(point2, point3, 1.0f);
static const geometry::Bulge bulge3(point3, point4, 0.0f);

TEST(BulgeTest, Length)
{
	EXPECT_FLOAT_EQ(bulge1.length(), point1.distanceToPoint(point2));
	EXPECT_FLOAT_EQ(bulge2.length(), point2.distanceToPoint(point3) / 2.0f * M_PI);
	EXPECT_FLOAT_EQ(bulge3.length(), point3.distanceToPoint(point4));
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

TEST(BulgeTest, LineBoundingRect)
{
	const geometry::Rect box = bulge1.boundingRect();
	EXPECT_FLOAT_EQ(1.2, box.min().x());
	EXPECT_FLOAT_EQ(4.5, box.max().x());
	EXPECT_FLOAT_EQ(3.4, box.min().y());
	EXPECT_FLOAT_EQ(6.7, box.max().y());
}

TEST(BulgeTest, ArcBoundingRect)
{
	const geometry::Rect box = bulge2.boundingRect();
	EXPECT_FLOAT_EQ(4.5, box.min().x());
	EXPECT_FLOAT_EQ(8.19022083, box.max().x());
	EXPECT_FLOAT_EQ(5.85977936, box.min().y());
	EXPECT_FLOAT_EQ(9.1, box.max().y());
}

