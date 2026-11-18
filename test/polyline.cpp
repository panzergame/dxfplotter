#include <gtest/gtest.h>

#include <cmath>
#include <QVector2D>
#include <polylineutils.h>

import geometry.bulge;
import geometry.circle;
import geometry.polyline;
import geometry.utils;

constexpr QVector2D point1(1.2, 3.4);
constexpr QVector2D point2(4.5, 6.7);
constexpr QVector2D point3(7.8, 9.1);
constexpr QVector2D point4(11.0, 12.0);

static const geometry::Bulge pointbulge(point1, point1, 0.0f);
static const geometry::Bulge bulge1(point1, point2, 0.0f);
static const geometry::Bulge bulge1next(point2, point3, 0.0f);
static const geometry::Bulge bulge1invert(point2, point1, 0.0f);
static const geometry::Bulge bulge2(point3, point4, 0.0f);

TEST(PolylineTest, WithEndAndStartEqualsAndOneBulgeIsPoint)
{
	const geometry::Polyline polyline1({pointbulge});
	EXPECT_TRUE(polyline1.isPoint());

	const geometry::Polyline polyline2({bulge1, bulge1invert});
	EXPECT_FALSE(polyline2.isPoint());
}

TEST(PolylineTest, WithBulgeAndInvertBulgeIsClosed)
{
	const geometry::Polyline polyline1({bulge1, bulge1invert});
	EXPECT_TRUE(polyline1.isClosed());

	const geometry::Polyline polyline2({bulge1invert, bulge1});
	EXPECT_TRUE(polyline2.isClosed());

	const geometry::Polyline polyline3({bulge1});
	EXPECT_FALSE(polyline3.isClosed());

	const geometry::Polyline polyline4({bulge1, bulge2});
	EXPECT_FALSE(polyline4.isClosed());
}

TEST(PolylineTest, ClosedPolylineHasStartEqualsToEnd)
{
	const geometry::Polyline polyline1({bulge1, bulge1invert});
	EXPECT_EQ(polyline1.start(), polyline1.end());
}

TEST(PolylineTest, StartEndMatchBulgeStartEnd)
{
	const geometry::Polyline polyline({bulge1, bulge2});
	EXPECT_EQ(polyline.start(), bulge1.start());
	EXPECT_EQ(polyline.end(), bulge2.end());
}

TEST(PolylineTest, InversePolyline)
{
	const geometry::Polyline polyline({bulge1, bulge2});
	const geometry::Polyline polylineInverse(polyline.inverse());

	EXPECT_EQ(polyline.start(), polylineInverse.end());
	EXPECT_EQ(polyline.end(), polylineInverse.start());
}

TEST(PolylineTest, ConcatTwoSingleBulgePolylineEqualsTwoBulgePolyline)
{
	const geometry::Polyline polyline1({bulge1});
	const geometry::Polyline polyline2({bulge1next});
	const geometry::Polyline polyline3({bulge1, bulge1next});
	geometry::Polyline polyline4;
	polyline4 += polyline1;
	polyline4 += polyline2;

	EXPECT_EQ(polyline3, polyline4);
}

TEST(PolylineTest, IterateBulge)
{
	const geometry::Bulge::List bulges{bulge1, bulge1next};
	geometry::Bulge::List bulges2(bulges);
	const geometry::Polyline polyline(std::move(bulges2));

	int i = 0;
	polyline.forEachBulge([&i, &bulges](const geometry::Bulge& bulge){
		EXPECT_EQ(bulge, bulges[i]);
		++i;
	});
}

TEST(PolylineTest, ScaleTransformIterateBulgeScaled)
{
	const geometry::Bulge::List bulges{bulge1, bulge1next};
	geometry::Bulge::List bulges2(bulges);
	geometry::Polyline polyline(std::move(bulges2));

	polyline.transformBulge([](geometry::Bulge& bulge){
		bulge.start() *= 0.1;
		bulge.end() *= 0.1;
	});

	int i = 0;
	polyline.forEachBulge([&i, &bulges](const geometry::Bulge& bulge){
		EXPECT_FLOAT_EQ(bulge.start().x(), bulges[i].start().x() * 0.1);
		EXPECT_FLOAT_EQ(bulge.start().y(), bulges[i].start().y() * 0.1);
		EXPECT_FLOAT_EQ(bulge.end().x(), bulges[i].end().x() * 0.1);
		EXPECT_FLOAT_EQ(bulge.end().y(), bulges[i].end().y() * 0.1);
		++i;
	});
}

TEST(PolylineTest, TestLinePolylineOffsetedHasMovedStartEnd)
{
	const geometry::Bulge bulge(QVector2D(0.0f, 3.4f), QVector2D(5.0f, 3.4f), 0.0f);
	const geometry::Polyline polyline({bulge});

	const float offset = 1.2f;
	const geometry::Polyline::List offseted(polyline.offsetted(offset));

	ASSERT_EQ(offseted.size(), 1);

	const geometry::Polyline &offsetedPolyline = offseted.front();
	EXPECT_FLOAT_EQ(offsetedPolyline.start().x(), bulge.start().x());
	EXPECT_FLOAT_EQ(offsetedPolyline.end().x(), bulge.end().x());
	EXPECT_FLOAT_EQ(offsetedPolyline.start().y(), bulge.start().y() + offset);
	EXPECT_FLOAT_EQ(offsetedPolyline.end().y(), bulge.end().y() + offset);
}

TEST(PolylineTest, TestPointPolylineOffsetedIsPoint)
{
	const geometry::Bulge bulge(QVector2D(0.0f, 3.4f), QVector2D(0.0f, 3.4f), 0.0f);
	const geometry::Polyline polyline({bulge});

	const float offset = 1.2f;
	const geometry::Polyline::List offseted(polyline.offsetted(offset));

	ASSERT_EQ(offseted.size(), 1);

	const geometry::Polyline &offsetedPolyline = offseted.front();
	EXPECT_EQ(offsetedPolyline.start(), bulge.start());
	EXPECT_EQ(offsetedPolyline.end(), bulge.end());
}

TEST(PolylineTest, TestPolylineOrientation4Bulges)
{
	const geometry::Polyline polyline({
		geometry::Bulge(point1, point2, 0.0f),
		geometry::Bulge(point2, point3, 0.0f),
		geometry::Bulge(point3, point4, 0.0f),
		geometry::Bulge(point4, point1, 0.0f)
	});

	ASSERT_EQ(polyline.orientation(), geometry::Orientation::CW);

	const geometry::Polyline invertedPolyline = polyline.inverse();
	ASSERT_EQ(invertedPolyline.orientation(), geometry::Orientation::CCW);
}

TEST(PolylineTest, TestConcavePolylineOrientation)
{
	const geometry::Polyline polyline = createStartPolyline(5.0f, 10.0f, 10);

	ASSERT_EQ(polyline.orientation(), geometry::Orientation::CCW);

	const geometry::Polyline invertedPolyline = polyline.inverse();
	ASSERT_EQ(invertedPolyline.orientation(), geometry::Orientation::CW);
}

TEST(PolylineTest, TestEquals)
{
	const geometry::Polyline polyline1 = createStartPolyline(5.0f, 10.0f, 10);
	const geometry::Polyline polyline2 = createStartPolyline(5.0f, 10.0f, 10);
	const geometry::Polyline polyline3 = polyline1.inverse();

	ASSERT_EQ(polyline1, polyline2);
	ASSERT_TRUE(polyline1.equals(polyline1, false));
	ASSERT_TRUE(polyline1.equals(polyline2, false));
	ASSERT_TRUE(polyline1.equals(polyline3, true));
	ASSERT_FALSE(polyline1.equals(polyline3, false));
}

namespace
{

constexpr float bladeOffset = 0.5f;
constexpr float smallSwivelAngle = 0.1f;
const float quarterTurnBulge = std::tan(M_PI / 8.0);

void ExpectVectorNear(const QVector2D& actual, const QVector2D& expected)
{
	EXPECT_NEAR(actual.x(), expected.x(), 1e-5f);
	EXPECT_NEAR(actual.y(), expected.y(), 1e-5f);
}

void ExpectBulgeNear(const geometry::Bulge& actual, const QVector2D& start, const QVector2D& end, float tangent)
{
	ExpectVectorNear(actual.start(), start);
	ExpectVectorNear(actual.end(), end);
	EXPECT_NEAR(actual.tangent(), tangent, 1e-5f);
}

geometry::Bulge::List Bulges(const geometry::Polyline& polyline)
{
	geometry::Bulge::List bulges;
	polyline.forEachBulge([&bulges](const geometry::Bulge& bulge) {
		bulges.push_back(bulge);
	});
	return bulges;
}

const geometry::Polyline lPolyline({ geometry::Bulge(QVector2D(0, 0), QVector2D(10, 0), 0.0f),
									 geometry::Bulge(QVector2D(10, 0), QVector2D(10, 10), 0.0f) });

}

TEST(PolylineTest, DragKnifeLineIsShiftedForward)
{
	const geometry::Polyline polyline({ geometry::Bulge(QVector2D(0, 0), QVector2D(10, 0), 0.0f) });

	const geometry::Bulge::List bulges = Bulges(polyline.dragKnifeCompensated(bladeOffset, smallSwivelAngle));

	ASSERT_EQ(bulges.size(), 1);
	ExpectBulgeNear(bulges[0], QVector2D(0.5, 0), QVector2D(10.5, 0), 0.0f);
}

TEST(PolylineTest, DragKnifeCornerAddsSwivelAroundVertex)
{
	const geometry::Bulge::List bulges = Bulges(lPolyline.dragKnifeCompensated(bladeOffset, smallSwivelAngle));

	ASSERT_EQ(bulges.size(), 3);
	ExpectBulgeNear(bulges[0], QVector2D(0.5, 0), QVector2D(10.5, 0), 0.0f);
	ExpectBulgeNear(bulges[1], QVector2D(10.5, 0), QVector2D(10, 0.5), quarterTurnBulge);
	ExpectBulgeNear(bulges[2], QVector2D(10, 0.5), QVector2D(10, 10.5), 0.0f);
}

TEST(PolylineTest, DragKnifeCornerBelowMinimumSwivelAngleJoinsBulges)
{
	const geometry::Bulge::List bulges = Bulges(lPolyline.dragKnifeCompensated(bladeOffset, M_PI_2 + 0.1f));

	ASSERT_EQ(bulges.size(), 2);
	ExpectBulgeNear(bulges[0], QVector2D(0.5, 0), QVector2D(10.5, 0), 0.0f);
	ExpectBulgeNear(bulges[1], QVector2D(10.5, 0), QVector2D(10, 10.5), 0.0f);
}

TEST(PolylineTest, DragKnifeArcKeepsCenterAndSweep)
{
	const geometry::Polyline polyline({ geometry::Bulge(QVector2D(1, 0), QVector2D(0, 1), quarterTurnBulge) });

	const geometry::Bulge::List bulges = Bulges(polyline.dragKnifeCompensated(bladeOffset, smallSwivelAngle));

	ASSERT_EQ(bulges.size(), 1);
	ExpectBulgeNear(bulges[0], QVector2D(1, 0.5), QVector2D(-0.5, 1), quarterTurnBulge);

	const geometry::Circle circle = bulges[0].toCircle();
	ExpectVectorNear(circle.center(), QVector2D(0, 0));
	EXPECT_NEAR(circle.radius(), std::sqrt(1.0f + bladeOffset * bladeOffset), 1e-5f);
}

TEST(PolylineTest, DragKnifeClosedPolylineSwivelsAtClosingVertex)
{
	const geometry::Polyline polyline({ geometry::Bulge(QVector2D(0, 0), QVector2D(10, 0), 0.0f),
										geometry::Bulge(QVector2D(10, 0), QVector2D(10, 10), 0.0f),
										geometry::Bulge(QVector2D(10, 10), QVector2D(0, 10), 0.0f),
										geometry::Bulge(QVector2D(0, 10), QVector2D(0, 0), 0.0f) });

	const geometry::Polyline compensated = polyline.dragKnifeCompensated(bladeOffset, smallSwivelAngle);
	EXPECT_TRUE(compensated.isClosed());

	const geometry::Bulge::List bulges = Bulges(compensated);
	ASSERT_EQ(bulges.size(), 8);
	ExpectBulgeNear(bulges[7], QVector2D(0, -0.5), QVector2D(0.5, 0), quarterTurnBulge);
}

TEST(PolylineTest, DragKnifeTangentContinuousBulgesHaveNoSwivel)
{
	const geometry::Polyline polyline({ geometry::Bulge(QVector2D(0, 0), QVector2D(1, 0), 0.0f),
										geometry::Bulge(QVector2D(1, 0), QVector2D(2, 1), quarterTurnBulge) });

	const geometry::Bulge::List bulges = Bulges(polyline.dragKnifeCompensated(bladeOffset, 0.0f));

	ASSERT_EQ(bulges.size(), 2);
	ExpectBulgeNear(bulges[0], QVector2D(0.5, 0), QVector2D(1.5, 0), 0.0f);
	ExpectBulgeNear(bulges[1], QVector2D(1.5, 0), QVector2D(2, 1.5), quarterTurnBulge);
}

TEST(PolylineTest, DragKnifePointIsUnchanged)
{
	const geometry::Polyline polyline({ geometry::Bulge(QVector2D(1, 2), QVector2D(1, 2), 0.0f) });

	const geometry::Bulge::List bulges = Bulges(polyline.dragKnifeCompensated(bladeOffset, smallSwivelAngle));

	ASSERT_EQ(bulges.size(), 1);
	ExpectBulgeNear(bulges[0], QVector2D(1, 2), QVector2D(1, 2), 0.0f);
}
