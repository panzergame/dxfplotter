#include <gtest/gtest.h>
#include <geometry/bulge.h>

constexpr QVector2D point1(-1.0f, -0.5f);
constexpr QVector2D point2(1.0f, 0.5f);

TEST(ArcTest, HalfCcwCircleBulgeConvertToArcMatchBulge)
{
	const geometry::Bulge bulge(point1, point2, 1.0f);
	const geometry::Arc arc(bulge.toArc());

	EXPECT_EQ(arc.orientation(), geometry::Orientation::CCW);

	EXPECT_NEAR(arc.center().x(), (point1.x() + point2.x()) / 2.0f, 1e-6);
	EXPECT_NEAR(arc.center().y(), (point1.y() + point2.y()) / 2.0f, 1e-6);

	EXPECT_NEAR(arc.radius(), point1.distanceToPoint(point2) / 2.0f, 1e-6);

	const float angle = std::atan2(point2.y(), point2.x());
	EXPECT_NEAR(arc.startAngle(), -M_PI + angle, 1e-6);
	EXPECT_NEAR(arc.endAngle(), angle, 1e-6);

	EXPECT_NEAR(arc.spanAngle(), M_PI, 1e-6);

	EXPECT_EQ(arc.start(), bulge.start());
	EXPECT_EQ(arc.end(), bulge.end());
}

TEST(ArcTest, HalfCwCircleBulgeConvertToArcMatchBulge)
{
	const geometry::Bulge bulge(point1, point2, -1.0f);
	const geometry::Arc arc(bulge.toArc());

	EXPECT_EQ(arc.orientation(), geometry::Orientation::CW);

	EXPECT_NEAR(arc.center().x(), (point1.x() + point2.x()) / 2.0f, 1e-6);
	EXPECT_NEAR(arc.center().y(), (point1.y() + point2.y()) / 2.0f, 1e-6);

	EXPECT_NEAR(arc.radius(), point1.distanceToPoint(point2) / 2.0f, 1e-6);

	const float angle = std::atan2(point2.y(), point2.x());
	EXPECT_NEAR(arc.startAngle(), M_PI + angle, 1e-6);
	EXPECT_NEAR(arc.endAngle(), angle, 1e-6);

	EXPECT_NEAR(arc.spanAngle(), -M_PI, 1e-6);

	EXPECT_EQ(arc.start(), bulge.start());
	EXPECT_EQ(arc.end(), bulge.end());
}
