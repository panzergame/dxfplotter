#include <gtest/gtest.h>

#include <cmath>
#include <QVector2D>

import geometry.bulge;
import geometry.polyline;
import geometry.tooloperation.dragknife;
import geometry.tooloperation.tooloperation;
import geometry.utils;

namespace
{

constexpr float bladeOffset = 0.5f;
constexpr float smallSwivelAngle = 0.1f;
constexpr float noAlignment = 0.0f;
const float quarterTurnBulge = std::tan(M_PI / 8.0);

const geometry::Polyline lPolyline({ geometry::Bulge(QVector2D(0, 0), QVector2D(10, 0), 0.0f),
									 geometry::Bulge(QVector2D(10, 0), QVector2D(10, 10), 0.0f) });

const geometry::Polyline ccwSquare({ geometry::Bulge(QVector2D(0, 0), QVector2D(10, 0), 0.0f),
									 geometry::Bulge(QVector2D(10, 0), QVector2D(10, 10), 0.0f),
									 geometry::Bulge(QVector2D(10, 10), QVector2D(0, 10), 0.0f),
									 geometry::Bulge(QVector2D(0, 10), QVector2D(0, 0), 0.0f) });

// Starts at its reflex vertex (5, 5).
const geometry::Polyline ccwLShape({ geometry::Bulge(QVector2D(5, 5), QVector2D(5, 10), 0.0f),
									 geometry::Bulge(QVector2D(5, 10), QVector2D(0, 10), 0.0f),
									 geometry::Bulge(QVector2D(0, 10), QVector2D(0, 0), 0.0f),
									 geometry::Bulge(QVector2D(0, 0), QVector2D(10, 0), 0.0f),
									 geometry::Bulge(QVector2D(10, 0), QVector2D(10, 5), 0.0f),
									 geometry::Bulge(QVector2D(10, 5), QVector2D(5, 5), 0.0f) });

const geometry::Polyline ccwCircle({ geometry::Bulge(QVector2D(10, 0), QVector2D(-10, 0), 1.0f),
									 geometry::Bulge(QVector2D(-10, 0), QVector2D(10, 0), 1.0f) });

constexpr float alignmentLength = 2.0f;
const float threeEighthTurnBulge = std::tan(3.0 * M_PI / 16.0);

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

// Waste side is the left of the polyline for a FORWARD side, travel is forward when side and profile match.
geometry::tooloperation::ToolPolyline AlignedToolPolyline(const geometry::Polyline& polyline,
														  geometry::CuttingDirection sideDirection,
														  geometry::CuttingDirection profileDirection)
{
	const geometry::tooloperation::DragKnife dragKnife(bladeOffset, smallSwivelAngle, alignmentLength, sideDirection,
													   profileDirection);

	const geometry::tooloperation::DragKnife::Result result = dragKnife.generate(polyline);

	EXPECT_EQ(result.polylines.size(), 1);
	const geometry::tooloperation::ToolPolyline& toolPolyline = result.polylines.front();
	EXPECT_TRUE(toolPolyline.entry);
	if (toolPolyline.entry) {
		ExpectVectorNear(toolPolyline.entry->end(), toolPolyline.polyline.start());
	}

	return toolPolyline;
}

}

TEST(DragKnifeTest, ForwardIsCompensatedInPolylineDirection)
{
	const geometry::tooloperation::DragKnife dragKnife(bladeOffset, smallSwivelAngle, noAlignment,
													   geometry::CuttingDirection::FORWARD,
													   geometry::CuttingDirection::FORWARD);

	const geometry::tooloperation::DragKnife::Result result = dragKnife.generate(lPolyline);

	EXPECT_EQ(result.cuttingDirection, geometry::CuttingDirection::FORWARD);
	ASSERT_EQ(result.polylines.size(), 1);

	const geometry::Bulge::List bulges = Bulges(result.polylines.front().polyline);
	ASSERT_EQ(bulges.size(), 3);
	ExpectBulgeNear(bulges[0], QVector2D(0.5, 0), QVector2D(10.5, 0), 0.0f);
	ExpectBulgeNear(bulges[1], QVector2D(10.5, 0), QVector2D(10, 0.5), quarterTurnBulge);
	ExpectBulgeNear(bulges[2], QVector2D(10, 0.5), QVector2D(10, 10.5), 0.0f);
}

TEST(DragKnifeTest, BackwardIsCompensatedInCuttingDirection)
{
	const geometry::tooloperation::DragKnife dragKnife(bladeOffset, smallSwivelAngle, noAlignment,
													   geometry::CuttingDirection::FORWARD,
													   geometry::CuttingDirection::BACKWARD);

	const geometry::tooloperation::DragKnife::Result result = dragKnife.generate(lPolyline);

	// Same direction as the profile so that inversion at export cancels the one done before compensation.
	EXPECT_EQ(result.cuttingDirection, geometry::CuttingDirection::BACKWARD);
	ASSERT_EQ(result.polylines.size(), 1);

	const geometry::Bulge::List bulges = Bulges(result.polylines.front().polyline);
	ASSERT_EQ(bulges.size(), 3);
	ExpectBulgeNear(bulges[0], QVector2D(10, 9.5), QVector2D(10, -0.5), 0.0f);
	ExpectBulgeNear(bulges[1], QVector2D(10, -0.5), QVector2D(9.5, 0), -quarterTurnBulge);
	ExpectBulgeNear(bulges[2], QVector2D(9.5, 0), QVector2D(-0.5, 0), 0.0f);
}

TEST(DragKnifeTest, BackwardSideIsCompensatedAgainstPolylineDirection)
{
	const geometry::tooloperation::DragKnife dragKnife(bladeOffset, smallSwivelAngle, noAlignment,
													   geometry::CuttingDirection::BACKWARD,
													   geometry::CuttingDirection::FORWARD);

	const geometry::tooloperation::DragKnife::Result result = dragKnife.generate(lPolyline);

	EXPECT_EQ(result.cuttingDirection, geometry::CuttingDirection::FORWARD);
	ASSERT_EQ(result.polylines.size(), 1);

	const geometry::Bulge::List bulges = Bulges(result.polylines.front().polyline);
	ASSERT_EQ(bulges.size(), 3);
	ExpectBulgeNear(bulges[0], QVector2D(10, 9.5), QVector2D(10, -0.5), 0.0f);
	ExpectBulgeNear(bulges[1], QVector2D(10, -0.5), QVector2D(9.5, 0), -quarterTurnBulge);
	ExpectBulgeNear(bulges[2], QVector2D(9.5, 0), QVector2D(-0.5, 0), 0.0f);
}

TEST(DragKnifeTest, BackwardSideAndBackwardProfileCancel)
{
	const geometry::tooloperation::DragKnife dragKnife(bladeOffset, smallSwivelAngle, noAlignment,
													   geometry::CuttingDirection::BACKWARD,
													   geometry::CuttingDirection::BACKWARD);

	const geometry::tooloperation::DragKnife::Result result = dragKnife.generate(lPolyline);

	EXPECT_EQ(result.cuttingDirection, geometry::CuttingDirection::BACKWARD);
	ASSERT_EQ(result.polylines.size(), 1);

	const geometry::Bulge::List bulges = Bulges(result.polylines.front().polyline);
	ASSERT_EQ(bulges.size(), 3);
	ExpectBulgeNear(bulges[0], QVector2D(0.5, 0), QVector2D(10.5, 0), 0.0f);
	ExpectBulgeNear(bulges[1], QVector2D(10.5, 0), QVector2D(10, 0.5), quarterTurnBulge);
	ExpectBulgeNear(bulges[2], QVector2D(10, 0.5), QVector2D(10, 10.5), 0.0f);
}

TEST(DragKnifeTest, AlignmentEndsAtHolderStartAlongFirstTangent)
{
	const geometry::tooloperation::DragKnife dragKnife(bladeOffset, smallSwivelAngle, 2.0f,
													   geometry::CuttingDirection::FORWARD,
													   geometry::CuttingDirection::FORWARD);

	const geometry::tooloperation::DragKnife::Result result = dragKnife.generate(lPolyline);

	ASSERT_EQ(result.polylines.size(), 1);
	const geometry::tooloperation::ToolPolyline& toolPolyline = result.polylines.front();
	ASSERT_TRUE(toolPolyline.entry);

	const geometry::Bulge::List bulges = Bulges(*toolPolyline.entry);
	ASSERT_EQ(bulges.size(), 1);
	ExpectBulgeNear(bulges[0], QVector2D(-1.5, 0), QVector2D(0.5, 0), 0.0f);
	ExpectVectorNear(toolPolyline.entry->end(), toolPolyline.polyline.start());
}

TEST(DragKnifeTest, AlignmentFollowsArcStartTangent)
{
	const geometry::Polyline arcPolyline({ geometry::Bulge(QVector2D(10, 0), QVector2D(0, 10), quarterTurnBulge) });
	const geometry::tooloperation::DragKnife dragKnife(bladeOffset, smallSwivelAngle, 2.0f,
													   geometry::CuttingDirection::FORWARD,
													   geometry::CuttingDirection::FORWARD);

	const geometry::tooloperation::DragKnife::Result result = dragKnife.generate(arcPolyline);

	ASSERT_EQ(result.polylines.size(), 1);
	const geometry::tooloperation::ToolPolyline& toolPolyline = result.polylines.front();
	ASSERT_TRUE(toolPolyline.entry);

	const geometry::Bulge::List bulges = Bulges(*toolPolyline.entry);
	ASSERT_EQ(bulges.size(), 1);
	ExpectBulgeNear(bulges[0], QVector2D(10, -1.5), QVector2D(10, 0.5), 0.0f);
	ExpectVectorNear(toolPolyline.entry->end(), toolPolyline.polyline.start());
}

TEST(DragKnifeTest, AlignmentFollowsCuttingDirection)
{
	const geometry::tooloperation::DragKnife dragKnife(bladeOffset, smallSwivelAngle, 2.0f,
													   geometry::CuttingDirection::FORWARD,
													   geometry::CuttingDirection::BACKWARD);

	const geometry::tooloperation::DragKnife::Result result = dragKnife.generate(lPolyline);

	ASSERT_EQ(result.polylines.size(), 1);
	const geometry::tooloperation::ToolPolyline& toolPolyline = result.polylines.front();
	ASSERT_TRUE(toolPolyline.entry);

	const geometry::Bulge::List bulges = Bulges(*toolPolyline.entry);
	ASSERT_EQ(bulges.size(), 1);
	ExpectBulgeNear(bulges[0], QVector2D(10, 11.5), QVector2D(10, 9.5), 0.0f);
	ExpectVectorNear(toolPolyline.entry->end(), toolPolyline.polyline.start());
}

TEST(DragKnifeTest, NullAlignmentLengthHasNoEntry)
{
	const geometry::tooloperation::DragKnife dragKnife(bladeOffset, smallSwivelAngle, noAlignment,
													   geometry::CuttingDirection::FORWARD,
													   geometry::CuttingDirection::FORWARD);

	const geometry::tooloperation::DragKnife::Result result = dragKnife.generate(lPolyline);

	ASSERT_EQ(result.polylines.size(), 1);
	EXPECT_FALSE(result.polylines.front().entry);
}

TEST(DragKnifeTest, ConvexStartWithOutsideWasteAlignsAlongFirstTangent)
{
	const geometry::tooloperation::ToolPolyline toolPolyline = AlignedToolPolyline(
		ccwSquare, geometry::CuttingDirection::BACKWARD, geometry::CuttingDirection::BACKWARD);
	ASSERT_TRUE(toolPolyline.entry);

	const geometry::Bulge::List bulges = Bulges(*toolPolyline.entry);
	ASSERT_EQ(bulges.size(), 1);
	ExpectBulgeNear(bulges[0], QVector2D(-1.5, 0), QVector2D(0.5, 0), 0.0f);
}

TEST(DragKnifeTest, ConvexStartWithInsideWasteAlignsAlongBisectorThenSwivels)
{
	const geometry::tooloperation::ToolPolyline toolPolyline = AlignedToolPolyline(
		ccwSquare, geometry::CuttingDirection::FORWARD, geometry::CuttingDirection::FORWARD);
	ASSERT_TRUE(toolPolyline.entry);

	// Blade tip comes from (1.414, 1.414) inside the square, along the corner bisector.
	const float diagonalOffset = bladeOffset / std::sqrt(2.0f);
	const float diagonalStroke = alignmentLength / std::sqrt(2.0f);
	const geometry::Bulge::List bulges = Bulges(*toolPolyline.entry);
	ASSERT_EQ(bulges.size(), 2);
	ExpectBulgeNear(bulges[0], QVector2D(diagonalStroke - diagonalOffset, diagonalStroke - diagonalOffset),
					QVector2D(-diagonalOffset, -diagonalOffset), 0.0f);
	ExpectBulgeNear(bulges[1], QVector2D(-diagonalOffset, -diagonalOffset), QVector2D(0.5, 0), threeEighthTurnBulge);
}

TEST(DragKnifeTest, ReflexStartWithOutsideWasteAlignsAlongBisectorThenSwivels)
{
	const geometry::tooloperation::ToolPolyline toolPolyline = AlignedToolPolyline(
		ccwLShape, geometry::CuttingDirection::BACKWARD, geometry::CuttingDirection::BACKWARD);
	ASSERT_TRUE(toolPolyline.entry);

	// Blade tip comes from (6.414, 6.414) in the notch of the L.
	const float diagonalOffset = bladeOffset / std::sqrt(2.0f);
	const float diagonalStroke = alignmentLength / std::sqrt(2.0f);
	const geometry::Bulge::List bulges = Bulges(*toolPolyline.entry);
	ASSERT_EQ(bulges.size(), 2);
	ExpectBulgeNear(bulges[0], QVector2D(5 + diagonalStroke - diagonalOffset, 5 + diagonalStroke - diagonalOffset),
					QVector2D(5 - diagonalOffset, 5 - diagonalOffset), 0.0f);
	ExpectBulgeNear(bulges[1], QVector2D(5 - diagonalOffset, 5 - diagonalOffset), QVector2D(5, 5.5),
					-threeEighthTurnBulge);
}

TEST(DragKnifeTest, CircleWithOutsideWasteAlignsAlongTangent)
{
	const geometry::tooloperation::ToolPolyline toolPolyline = AlignedToolPolyline(
		ccwCircle, geometry::CuttingDirection::BACKWARD, geometry::CuttingDirection::BACKWARD);
	ASSERT_TRUE(toolPolyline.entry);

	const geometry::Bulge::List bulges = Bulges(*toolPolyline.entry);
	ASSERT_EQ(bulges.size(), 1);
	ExpectBulgeNear(bulges[0], QVector2D(10, -1.5), QVector2D(10, 0.5), 0.0f);
}

TEST(DragKnifeTest, CircleWithInsideWasteAlignsAlongRadiusThenSwivels)
{
	const geometry::tooloperation::ToolPolyline toolPolyline = AlignedToolPolyline(
		ccwCircle, geometry::CuttingDirection::FORWARD, geometry::CuttingDirection::FORWARD);
	ASSERT_TRUE(toolPolyline.entry);

	// Blade tip comes from (8, 0) inside the circle.
	const geometry::Bulge::List bulges = Bulges(*toolPolyline.entry);
	ASSERT_EQ(bulges.size(), 2);
	ExpectBulgeNear(bulges[0], QVector2D(8.5, 0), QVector2D(10.5, 0), 0.0f);
	ExpectBulgeNear(bulges[1], QVector2D(10.5, 0), QVector2D(10, 0.5), quarterTurnBulge);
}

TEST(DragKnifeTest, CircleTravelledBackwardKeepsWasteSideOfPolyline)
{
	// Travel is clockwise, waste stays on the left of the polyline: inside the circle.
	const geometry::tooloperation::ToolPolyline toolPolyline = AlignedToolPolyline(
		ccwCircle, geometry::CuttingDirection::FORWARD, geometry::CuttingDirection::BACKWARD);
	ASSERT_TRUE(toolPolyline.entry);

	const geometry::Bulge::List bulges = Bulges(*toolPolyline.entry);
	ASSERT_EQ(bulges.size(), 2);
	ExpectBulgeNear(bulges[0], QVector2D(8.5, 0), QVector2D(10.5, 0), 0.0f);
	ExpectBulgeNear(bulges[1], QVector2D(10.5, 0), QVector2D(10, -0.5), -quarterTurnBulge);
}

TEST(DragKnifeTest, OpenPolylineAlignsAlongFirstTangentWhateverWasteSide)
{
	for (const geometry::CuttingDirection side :
		 { geometry::CuttingDirection::FORWARD, geometry::CuttingDirection::BACKWARD }) {
		const geometry::tooloperation::ToolPolyline toolPolyline = AlignedToolPolyline(lPolyline, side, side);
		ASSERT_TRUE(toolPolyline.entry);

		const geometry::Bulge::List bulges = Bulges(*toolPolyline.entry);
		ASSERT_EQ(bulges.size(), 1);
		ExpectBulgeNear(bulges[0], QVector2D(-1.5, 0), QVector2D(0.5, 0), 0.0f);
	}
}
