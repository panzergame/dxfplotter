#include <gtest/gtest.h>

#include <algorithm>
#include <QVector2D>
#include <polylineutils.h>

import geometry.bulge;
import geometry.polyline;
import geometry.rect;
import geometry.tooloperation.pocket;
import geometry.utils;

namespace
{

constexpr float radius = 1.0f;
constexpr float minimumPolylineLength = 0.1f;
constexpr float noMinimumArcLength = 0.0f;
constexpr float epsilon = 1e-3f;

const geometry::Polyline ccwBorder = createSquarePolyline(QVector2D(0, 0), 10.0f);

float DistanceToRect(const QVector2D& point, const QVector2D& min, const QVector2D& max)
{
	const float dx = std::max({ min.x() - point.x(), 0.0f, point.x() - max.x() });
	const float dy = std::max({ min.y() - point.y(), 0.0f, point.y() - max.y() });
	return QVector2D(dx, dy).length();
}

}

TEST(PocketTest, CcwBorderIsCutForward)
{
	ASSERT_EQ(ccwBorder.orientation(), geometry::Orientation::CCW);

	const geometry::tooloperation::Pocket pocket({}, radius, minimumPolylineLength, noMinimumArcLength);

	const geometry::tooloperation::Pocket::Result result = pocket.generate(ccwBorder);

	ASSERT_FALSE(result.polylines.empty());
	EXPECT_EQ(result.cuttingDirection, geometry::CuttingDirection::FORWARD);
}

TEST(PocketTest, CwBorderIsCutBackward)
{
	const geometry::Polyline cwBorder = ccwBorder.inverse();
	ASSERT_EQ(cwBorder.orientation(), geometry::Orientation::CW);

	const geometry::tooloperation::Pocket pocket({}, radius, minimumPolylineLength, noMinimumArcLength);

	const geometry::tooloperation::Pocket::Result result = pocket.generate(cwBorder);

	ASSERT_FALSE(result.polylines.empty());
	EXPECT_EQ(result.cuttingDirection, geometry::CuttingDirection::BACKWARD);
}

TEST(PocketTest, PolylinesStayInsideBorderShrunkByRadius)
{
	const geometry::tooloperation::Pocket pocket({}, radius, minimumPolylineLength, noMinimumArcLength);

	const geometry::tooloperation::Pocket::Result result = pocket.generate(ccwBorder);

	ASSERT_FALSE(result.polylines.empty());
	for (const geometry::Polyline& polyline : result.polylines) {
		const geometry::Rect rect = polyline.boundingRect();
		EXPECT_GE(rect.min().x(), radius - epsilon);
		EXPECT_GE(rect.min().y(), radius - epsilon);
		EXPECT_LE(rect.max().x(), 10.0f - radius + epsilon);
		EXPECT_LE(rect.max().y(), 10.0f - radius + epsilon);
	}
}

TEST(PocketTest, PolylinesKeepRadiusAwayFromIsland)
{
	// Island distance to border is not a multiple of radius, otherwise border and island offsets coincide.
	const QVector2D islandMin(4.25, 4.25);
	const QVector2D islandMax(5.75, 5.75);
	const geometry::Polyline island = createSquarePolyline(islandMin, 1.5f);
	const geometry::tooloperation::Pocket pocket({ &island }, radius, minimumPolylineLength, noMinimumArcLength);

	const geometry::tooloperation::Pocket::Result result = pocket.generate(ccwBorder);

	ASSERT_FALSE(result.polylines.empty());
	for (const geometry::Polyline& polyline : result.polylines) {
		polyline.forEachBulge([&](const geometry::Bulge& bulge) {
			EXPECT_GE(DistanceToRect(bulge.start(), islandMin, islandMax), radius - epsilon);
		});
	}
}

TEST(PocketTest, RadiusLargerThanBorderIsEmpty)
{
	const geometry::Polyline smallBorder = createSquarePolyline(QVector2D(0, 0), 1.0f);
	const geometry::tooloperation::Pocket pocket({}, 2.0f, minimumPolylineLength, noMinimumArcLength);

	const geometry::tooloperation::Pocket::Result result = pocket.generate(smallBorder);

	EXPECT_TRUE(result.polylines.empty());
}
