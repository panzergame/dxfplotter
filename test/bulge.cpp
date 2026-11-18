#include <gtest/gtest.h>

#include <cmath>
#include <QVector2D>

import geometry.bulge;
import geometry.rect;

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

TEST(BulgeTest, EqualsInverse)
{
	const geometry::Bulge b1(point1, point2, 0.5f);
	const geometry::Bulge b2(point2, point1, -0.5f);
	
	EXPECT_TRUE(b1.equalsInversed(b2));
	EXPECT_FALSE(b1.equalsInversed(b1));
}

static void ExpectVectorNear(const QVector2D& actual, const QVector2D& expected)
{
	EXPECT_NEAR(actual.x(), expected.x(), 1e-5f);
	EXPECT_NEAR(actual.y(), expected.y(), 1e-5f);
}

TEST(BulgeTest, LineTangentsAreChordDirection)
{
	const QVector2D direction = (point2 - point1).normalized();

	ExpectVectorNear(bulge1.startTangent(), direction);
	ExpectVectorNear(bulge1.endTangent(), direction);
}

TEST(BulgeTest, CounterClockwiseHalfCircleTangents)
{
	const geometry::Bulge bulge(QVector2D(1, 0), QVector2D(-1, 0), 1.0f);

	ExpectVectorNear(bulge.startTangent(), QVector2D(0, 1));
	ExpectVectorNear(bulge.endTangent(), QVector2D(0, -1));
}

TEST(BulgeTest, ClockwiseHalfCircleTangents)
{
	const geometry::Bulge bulge(QVector2D(1, 0), QVector2D(-1, 0), -1.0f);

	ExpectVectorNear(bulge.startTangent(), QVector2D(0, -1));
	ExpectVectorNear(bulge.endTangent(), QVector2D(0, 1));
}

TEST(BulgeTest, CounterClockwiseQuarterCircleTangents)
{
	const geometry::Bulge bulge(QVector2D(1, 0), QVector2D(0, 1), std::tan(M_PI / 8.0));

	ExpectVectorNear(bulge.startTangent(), QVector2D(0, 1));
	ExpectVectorNear(bulge.endTangent(), QVector2D(-1, 0));
}
