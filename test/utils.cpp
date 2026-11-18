#include <gtest/gtest.h>

#include <cmath>
#include <QVector2D>

import geometry.utils;

static void ExpectVectorNear(const QVector2D& actual, const QVector2D& expected)
{
	EXPECT_NEAR(actual.x(), expected.x(), 1e-5f);
	EXPECT_NEAR(actual.y(), expected.y(), 1e-5f);
}

TEST(UtilsTest, RotateVectorCounterClockwise)
{
	ExpectVectorNear(geometry::RotateVector(QVector2D(1, 0), M_PI_2), QVector2D(0, 1));
}

TEST(UtilsTest, RotateVectorClockwise)
{
	ExpectVectorNear(geometry::RotateVector(QVector2D(1, 0), -M_PI_2), QVector2D(0, -1));
}

TEST(UtilsTest, SignedAngleCounterClockwiseIsPositive)
{
	EXPECT_FLOAT_EQ(geometry::SignedAngle(QVector2D(1, 0), QVector2D(0, 1)), M_PI_2);
}

TEST(UtilsTest, SignedAngleClockwiseIsNegative)
{
	EXPECT_FLOAT_EQ(geometry::SignedAngle(QVector2D(1, 0), QVector2D(0, -1)), -M_PI_2);
}

TEST(UtilsTest, SignedAngleSameVectorIsZero)
{
	EXPECT_FLOAT_EQ(geometry::SignedAngle(QVector2D(1, 0), QVector2D(2, 0)), 0.0f);
}

TEST(UtilsTest, SignedAngleOppositeVectorIsPi)
{
	EXPECT_FLOAT_EQ(std::abs(geometry::SignedAngle(QVector2D(1, 0), QVector2D(-1, 0))), M_PI);
}
