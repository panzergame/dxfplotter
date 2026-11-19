#include <gtest/gtest.h>

#include <cmath>
#include <QVector2D>

import geometry.bulge;
import geometry.polyline;
import geometry.tooloperation.dragknife;
import geometry.utils;

namespace
{

constexpr float bladeOffset = 0.5f;
constexpr float smallSwivelAngle = 0.1f;
const float quarterTurnBulge = std::tan(M_PI / 8.0);

const geometry::Polyline lPolyline({ geometry::Bulge(QVector2D(0, 0), QVector2D(10, 0), 0.0f),
									 geometry::Bulge(QVector2D(10, 0), QVector2D(10, 10), 0.0f) });

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

}

TEST(DragKnifeTest, ForwardIsCompensatedInPolylineDirection)
{
	const geometry::tooloperation::DragKnife dragKnife(bladeOffset, smallSwivelAngle,
													   geometry::CuttingDirection::FORWARD);

	const geometry::tooloperation::DragKnife::Result result = dragKnife.generate(lPolyline);

	EXPECT_EQ(result.cuttingDirection, geometry::CuttingDirection::FORWARD);
	ASSERT_EQ(result.polylines.size(), 1);

	const geometry::Bulge::List bulges = Bulges(result.polylines.front());
	ASSERT_EQ(bulges.size(), 3);
	ExpectBulgeNear(bulges[0], QVector2D(0.5, 0), QVector2D(10.5, 0), 0.0f);
	ExpectBulgeNear(bulges[1], QVector2D(10.5, 0), QVector2D(10, 0.5), quarterTurnBulge);
	ExpectBulgeNear(bulges[2], QVector2D(10, 0.5), QVector2D(10, 10.5), 0.0f);
}

TEST(DragKnifeTest, BackwardIsCompensatedInCuttingDirection)
{
	const geometry::tooloperation::DragKnife dragKnife(bladeOffset, smallSwivelAngle,
													   geometry::CuttingDirection::BACKWARD);

	const geometry::tooloperation::DragKnife::Result result = dragKnife.generate(lPolyline);

	// Same direction as the profile so that inversion at export cancels the one done before compensation.
	EXPECT_EQ(result.cuttingDirection, geometry::CuttingDirection::BACKWARD);
	ASSERT_EQ(result.polylines.size(), 1);

	const geometry::Bulge::List bulges = Bulges(result.polylines.front());
	ASSERT_EQ(bulges.size(), 3);
	ExpectBulgeNear(bulges[0], QVector2D(10, 9.5), QVector2D(10, -0.5), 0.0f);
	ExpectBulgeNear(bulges[1], QVector2D(10, -0.5), QVector2D(9.5, 0), -quarterTurnBulge);
	ExpectBulgeNear(bulges[2], QVector2D(9.5, 0), QVector2D(-0.5, 0), 0.0f);
}
