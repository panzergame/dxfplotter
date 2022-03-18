#include <gtest/gtest.h>
#include <geometry/pocketer.h>
#include <polylineutils.h>

TEST(PocketerTest, ShouldKeepBorderOrientationWhenBorderCcw)
{
	const geometry::Polyline border = createStartPolyline(5.0f, 10.0f, 20);
	const geometry::Orientation borderOrientation = border.orientation();
	geometry::Pocketer pocketer(border, {}, 1.0f, 0.1f);
	geometry::Polyline::List polylines = std::move(pocketer.polylines());

	for (const geometry::Polyline &polyline : polylines) {
		EXPECT_EQ(polyline.orientation(), borderOrientation);
	}
}

TEST(PocketerTest, ShouldKeepBorderOrientationWhenBorderCw)
{
	const geometry::Polyline border = createStartPolyline(5.0f, 10.0f, 20).inverse();
	const geometry::Orientation borderOrientation = border.orientation();
	geometry::Pocketer pocketer(border, {}, 1.0f, 0.1f);
	geometry::Polyline::List polylines = std::move(pocketer.polylines());

	for (const geometry::Polyline &polyline : polylines) {
		EXPECT_EQ(polyline.orientation(), borderOrientation);
	}
}

TEST(PocketerTest, ShouldKeepBorderOrientationWhenBorderCwAndIslandCcw)
{
	const geometry::Polyline border = createStartPolyline(5.0f, 10.0f, 20).inverse();
	const geometry::Polyline island = createStartPolyline(4.0f, 2.0f, 20);
	const geometry::Orientation borderOrientation = border.orientation();
	geometry::Pocketer pocketer(border, {&island}, 1.0f, 0.1f);
	geometry::Polyline::List polylines = std::move(pocketer.polylines());

	for (const geometry::Polyline &polyline : polylines) {
		EXPECT_EQ(polyline.orientation(), borderOrientation);
	}
}

TEST(PocketerTest, ShouldKeepBorderOrientationWhenBorderCwAndIslandCw)
{
	const geometry::Polyline border = createStartPolyline(5.0f, 10.0f, 20).inverse();
	const geometry::Polyline island = createStartPolyline(4.0f, 2.0f, 20).inverse();
	const geometry::Orientation borderOrientation = border.orientation();
	geometry::Pocketer pocketer(border, {&island}, 1.0f, 0.1f);
	geometry::Polyline::List polylines = std::move(pocketer.polylines());

	for (const geometry::Polyline &polyline : polylines) {
		EXPECT_EQ(polyline.orientation(), borderOrientation);
	}
}
