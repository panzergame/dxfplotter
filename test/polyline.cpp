#include <gtest/gtest.h>
#include <geometry/polyline.h>

TEST(PolylineTest, TestPolylineWithEndAndStartEqualsAndOneBulgeIsPoint)
{
	const QVector2D point1(1.2, 3.4);
	const Geometry::Polyline polyline1({Geometry::Bulge(point1, point1, 0.0f)});
	ASSERT_TRUE(polyline1.isPoint());

	const QVector2D point2(4.5, 6.7);
	const Geometry::Polyline polyline2({Geometry::Bulge(point1, point2, 0.0f), Geometry::Bulge(point2, point1, 0.0f)});
	ASSERT_FALSE(polyline2.isPoint());
}
