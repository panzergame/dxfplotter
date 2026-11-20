#include <gtest/gtest.h>

#include <QVector2D>
#include <polylineutils.h>

import geometry.bulge;
import geometry.polyline;
import geometry.rect;
import geometry.tooloperation.offset;
import geometry.utils;

namespace
{

constexpr float noMinimumLength = 0.0f;

// Positive margin offsets to the left of travel: outward for a clockwise border.
const geometry::Polyline cwSquare = createSquarePolyline(QVector2D(0, 0), 10.0f).inverse();

int ArcCount(const geometry::Polyline& polyline)
{
	int count = 0;
	polyline.forEachBulge([&count](const geometry::Bulge& bulge) {
		count += !bulge.isLine();
	});
	return count;
}

}

TEST(OffsetTest, PositiveMarginIsCutForward)
{
	const geometry::tooloperation::Offset offset(1.0f, noMinimumLength, noMinimumLength);

	const geometry::tooloperation::Offset::Result result = offset.generate(cwSquare);

	ASSERT_FALSE(result.polylines.empty());
	EXPECT_EQ(result.cuttingDirection, geometry::CuttingDirection::FORWARD);
}

TEST(OffsetTest, NegativeMarginIsCutBackward)
{
	const geometry::tooloperation::Offset offset(-1.0f, noMinimumLength, noMinimumLength);

	const geometry::tooloperation::Offset::Result result = offset.generate(cwSquare);

	ASSERT_FALSE(result.polylines.empty());
	EXPECT_EQ(result.cuttingDirection, geometry::CuttingDirection::BACKWARD);
}

TEST(OffsetTest, OutwardOffsetGrowsBoundingRect)
{
	ASSERT_EQ(cwSquare.orientation(), geometry::Orientation::CW);

	const geometry::tooloperation::Offset offset(1.0f, noMinimumLength, noMinimumLength);

	const geometry::tooloperation::Offset::Result result = offset.generate(cwSquare);

	ASSERT_EQ(result.polylines.size(), 1);
	const geometry::Polyline& polyline = result.polylines.front().polyline;
	EXPECT_TRUE(polyline.isClosed());

	const geometry::Rect rect = polyline.boundingRect();
	EXPECT_NEAR(rect.min().x(), -1.0f, 1e-4f);
	EXPECT_NEAR(rect.min().y(), -1.0f, 1e-4f);
	EXPECT_NEAR(rect.max().x(), 11.0f, 1e-4f);
	EXPECT_NEAR(rect.max().y(), 11.0f, 1e-4f);
}

TEST(OffsetTest, InwardOffsetCollapsingIsEmpty)
{
	const geometry::Polyline square = createSquarePolyline(QVector2D(0, 0), 2.0f).inverse();
	const geometry::tooloperation::Offset offset(-2.0f, noMinimumLength, noMinimumLength);

	const geometry::tooloperation::Offset::Result result = offset.generate(square);

	EXPECT_TRUE(result.polylines.empty());
}

TEST(OffsetTest, OutwardOffsetRoundsCorners)
{
	const geometry::tooloperation::Offset offset(1.0f, noMinimumLength, noMinimumLength);

	const geometry::tooloperation::Offset::Result result = offset.generate(cwSquare);

	ASSERT_EQ(result.polylines.size(), 1);
	EXPECT_EQ(ArcCount(result.polylines.front().polyline), 4);
}

TEST(OffsetTest, CornerArcsShorterThanMinimumArcLengthAreStraightened)
{
	// Each rounded corner is a quarter circle of radius 1, about 1.57 long.
	const geometry::tooloperation::Offset offset(1.0f, noMinimumLength, 2.0f);

	const geometry::tooloperation::Offset::Result result = offset.generate(cwSquare);

	ASSERT_EQ(result.polylines.size(), 1);
	EXPECT_EQ(ArcCount(result.polylines.front().polyline), 0);
}
