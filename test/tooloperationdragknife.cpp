#include <gtest/gtest.h>

#include <cmath>
#include <numbers>
#include <optional>
#include <QVector2D>

import geometry.bulge;
import geometry.polyline;
import geometry.tooloperation.dragknife;
import geometry.tooloperation.dragknifeentry;
import geometry.tooloperation.tooloperation;
import geometry.utils;

namespace
{

constexpr float pi = std::numbers::pi_v<float>;
constexpr float degree = pi / 180.0f;

constexpr float bladeOffset = 0.5f;
constexpr float smallSwivelAngle = 0.1f;
constexpr float flipTolerance = 0.1f * degree;
constexpr float alignmentTolerance = 1.0f * degree;
constexpr float clearanceTolerance = 0.05f * bladeOffset;
const float quarterTurnBulge = std::tan(M_PI / 8.0);

const geometry::tooloperation::DragKnifeLengths lengths
	= geometry::tooloperation::DragKnifeEntryLengths(bladeOffset, flipTolerance, alignmentTolerance);

const geometry::Polyline lPolyline({ geometry::Bulge(QVector2D(0, 0), QVector2D(10, 0), 0.0f),
									 geometry::Bulge(QVector2D(10, 0), QVector2D(10, 10), 0.0f) });

geometry::Polyline CcwPolygon(std::initializer_list<QVector2D> vertices)
{
	const std::vector<QVector2D> points(vertices);
	geometry::Bulge::List bulges;
	for (std::size_t i = 0; i < points.size(); ++i) {
		bulges.emplace_back(points[i], points[(i + 1) % points.size()], 0.0f);
	}
	return geometry::Polyline(std::move(bulges));
}

const geometry::Polyline ccwSquare
	= CcwPolygon({ QVector2D(0, 0), QVector2D(10, 0), QVector2D(10, 10), QVector2D(0, 10) });

// 2 r wide, too small for any entry inside.
const geometry::Polyline tinyCcwSquare
	= CcwPolygon({ QVector2D(0, 0), QVector2D(1, 0), QVector2D(1, 1), QVector2D(0, 1) });

const geometry::Polyline ccwHexagon
	= CcwPolygon({ QVector2D(10, 0), QVector2D(5, 8.660254f), QVector2D(-5, 8.660254f), QVector2D(-10, 0),
				   QVector2D(-5, -8.660254f), QVector2D(5, -8.660254f) });

const geometry::Polyline ccwCircle({ geometry::Bulge(QVector2D(10, 0), QVector2D(-10, 0), 1.0f),
									 geometry::Bulge(QVector2D(-10, 0), QVector2D(10, 0), 1.0f) });

void ExpectVectorNear(const QVector2D& actual, const QVector2D& expected, float error = 1e-5f)
{
	EXPECT_NEAR(actual.x(), expected.x(), error);
	EXPECT_NEAR(actual.y(), expected.y(), error);
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

QVector2D BulgePoint(const geometry::Bulge& bulge, float ratio)
{
	if (bulge.isLine()) {
		return bulge.start() + (bulge.end() - bulge.start()) * ratio;
	}

	const QVector2D tangent = bulge.startTangent();
	const float sweep = 4.0f * std::atan(bulge.tangent());
	const float curvature = 2.0f * std::sin(sweep / 2.0f) / bulge.start().distanceToPoint(bulge.end());
	const float angle = sweep * ratio;
	return bulge.start() + tangent * (std::sin(angle) / curvature)
		+ geometry::PerpendicularLine(tangent) * ((1.0f - std::cos(angle)) / curvature);
}

bool NearZone(const geometry::tooloperation::DragKnifeTipZone& zone, const QVector2D& point, float distance)
{
	if (zone.contains(point)) {
		return true;
	}
	for (int i = 0; i < 32; ++i) {
		const float angle = 2.0f * pi * i / 32;
		if (zone.contains(point + QVector2D(std::cos(angle), std::sin(angle)) * distance)) {
			return true;
		}
	}
	return false;
}

struct SimulationResult
{
	// Blade angle from the start tangent at the end of the entry.
	float error;
	bool tipsInZone;
};

/** Integrate the tractrix along the holder entry, exact per straight step:
 * tan(θ'/2) = tan(θ/2)·e^(−s/r), θ the blade (tip to pivot) angle from the step direction.
 * @param heading Initial blade angle from stroke A.
 */
SimulationResult SimulateEntry(const geometry::Polyline& entry, const geometry::tooloperation::DragKnifeTipZone& zone,
							   const QVector2D& startTangent, float heading)
{
	const geometry::Bulge::List bulges = Bulges(entry);
	const QVector2D firstDirection = (bulges.front().end() - bulges.front().start()).normalized();
	QVector2D pivot = bulges.front().start();
	QVector2D tip = pivot - geometry::RotateVector(firstDirection, heading) * bladeOffset;
	bool tipsInZone = true;

	for (std::size_t index = 0; index < bulges.size(); ++index) {
		const geometry::Bulge& bulge = bulges[index];
		const int steps = bulge.isLine() ? 200 : 1024;
		// The swivel keeps the tip on the start, only strokes A and B are bounded by the zone.
		const bool stroke = index < 2;

		for (int i = 1; i <= steps; ++i) {
			const QVector2D next = BulgePoint(bulge, float(i) / steps);
			const QVector2D move = next - pivot;
			const float length = move.length();
			const QVector2D direction = move / length;
			const QVector2D blade = (pivot - tip) / bladeOffset;

			const float half = 0.5f * geometry::SignedAngle(direction, blade);
			const float angle = 2.0f * std::atan2(std::exp(-length / bladeOffset) * std::sin(half), std::cos(half));
			pivot = next;
			tip = pivot - geometry::RotateVector(direction, angle) * bladeOffset;

			if (stroke && !NearZone(zone, tip, 1e-3f)) {
				tipsInZone = false;
			}
		}
	}

	return { std::abs(geometry::SignedAngle(startTangent, pivot - tip)), tipsInZone };
}

/// Entry built from the selected candidate, which must stay on the waste side and align every blade.
geometry::tooloperation::DragKnifeEntryCandidate ExpectValidEntry(const geometry::Polyline& contour, bool wasteOnLeft)
{
	const std::optional<geometry::tooloperation::DragKnifeEntryCandidate> candidate
		= geometry::tooloperation::DragKnifeEntrySelect(contour, wasteOnLeft, lengths);
	EXPECT_TRUE(candidate);
	if (!candidate) {
		return {};
	}

	const QVector2D leftNormal = geometry::PerpendicularLine(candidate->startTangent);
	const QVector2D waste = wasteOnLeft ? leftNormal : -leftNormal;
	// B arrives from the waste side.
	EXPECT_GE(QVector2D::dotProduct(-candidate->direction(), waste), -1e-5f);

	const geometry::Polyline entry = geometry::tooloperation::DragKnifeEntry(*candidate, lengths);
	const geometry::tooloperation::DragKnifeTipZone zone(*candidate, lengths, false);
	for (const float heading : { pi - flipTolerance, pi + flipTolerance, 0.0f, pi / 2.0f, -pi / 2.0f }) {
		const SimulationResult result = SimulateEntry(entry, zone, candidate->startTangent, heading);
		EXPECT_LT(result.error, alignmentTolerance + 0.05f * degree) << heading;
		EXPECT_TRUE(result.tipsInZone) << heading;
	}

	return *candidate;
}

// Waste side is the left of the polyline for a FORWARD side, travel is forward when side and profile match.
geometry::tooloperation::ToolPolyline GeneratedToolPolyline(const geometry::Polyline& polyline,
															geometry::CuttingDirection sideDirection,
															geometry::CuttingDirection profileDirection)
{
	const geometry::tooloperation::DragKnife dragKnife(bladeOffset, smallSwivelAngle, flipTolerance, alignmentTolerance,
													   sideDirection, profileDirection);

	const geometry::tooloperation::DragKnife::Result result = dragKnife.generate(polyline);

	EXPECT_EQ(result.polylines.size(), 1);
	const geometry::tooloperation::ToolPolyline& toolPolyline = result.polylines.front();
	EXPECT_TRUE(toolPolyline.entry);
	if (toolPolyline.entry) {
		ExpectVectorNear(toolPolyline.entry->end(), toolPolyline.polyline.start());
	}

	return toolPolyline;
}

// Candidate along +x ending on the origin, A coming from +y.
const geometry::tooloperation::DragKnifeEntryCandidate xCandidate { QVector2D(0, 0), QVector2D(1, 0), QVector2D(0, 1),
																	0.0f };
const float xStrokeBStart = bladeOffset - lengths.b;

bool XCandidateClear(const geometry::Polyline& contour, bool stuckBlades)
{
	return geometry::tooloperation::DragKnifeEntryClear(contour, xCandidate, lengths, clearanceTolerance, stuckBlades);
}

geometry::Polyline Segment(const QVector2D& start, const QVector2D& end)
{
	return geometry::Polyline({ geometry::Bulge(start, end, 0.0f) });
}

}

TEST(DragKnifeTest, ForwardIsCompensatedInPolylineDirection)
{
	const geometry::tooloperation::ToolPolyline toolPolyline
		= GeneratedToolPolyline(lPolyline, geometry::CuttingDirection::FORWARD, geometry::CuttingDirection::FORWARD);

	const geometry::Bulge::List bulges = Bulges(toolPolyline.polyline);
	ASSERT_EQ(bulges.size(), 3);
	ExpectBulgeNear(bulges[0], QVector2D(0.5, 0), QVector2D(10.5, 0), 0.0f);
	ExpectBulgeNear(bulges[1], QVector2D(10.5, 0), QVector2D(10, 0.5), quarterTurnBulge);
	ExpectBulgeNear(bulges[2], QVector2D(10, 0.5), QVector2D(10, 10.5), 0.0f);
}

TEST(DragKnifeTest, BackwardIsCompensatedInCuttingDirection)
{
	const geometry::tooloperation::DragKnife dragKnife(bladeOffset, smallSwivelAngle, flipTolerance, alignmentTolerance,
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
	const geometry::tooloperation::DragKnife dragKnife(bladeOffset, smallSwivelAngle, flipTolerance, alignmentTolerance,
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
	const geometry::tooloperation::DragKnife dragKnife(bladeOffset, smallSwivelAngle, flipTolerance, alignmentTolerance,
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

TEST(DragKnifeEntryTest, LengthsBalanceStrokesAtQuarterTurn)
{
	EXPECT_NEAR(lengths.a / bladeOffset, 7.925f, 1e-3f);
	EXPECT_NEAR(lengths.b / bladeOffset, 5.623f, 1e-3f);
	EXPECT_NEAR(lengths.flipExit, pi / 4.0f, 1e-5f);
	EXPECT_NEAR(lengths.c, std::log(1.0f + std::numbers::sqrt2_v<float>) * bladeOffset, 1e-5f);
	EXPECT_NEAR(lengths.a90, bladeOffset * std::log(1.0f / std::tan(flipTolerance / 2.0f)), 1e-5f);
}

TEST(DragKnifeEntryTest, EntryIsPerpendicularStrokesThenSwivel)
{
	const QVector2D start(3, 4);
	const QVector2D startTangent(0, 1);
	const QVector2D direction = geometry::RotateVector(startTangent, -pi / 3.0f);
	const geometry::tooloperation::DragKnifeEntryCandidate candidate { start, startTangent,
																	   geometry::PerpendicularLine(direction),
																	   pi / 3.0f };

	const geometry::Bulge::List bulges = Bulges(geometry::tooloperation::DragKnifeEntry(candidate, lengths));
	ASSERT_EQ(bulges.size(), 3);

	const QVector2D strokeA = bulges[0].end() - bulges[0].start();
	const QVector2D strokeB = bulges[1].end() - bulges[1].start();
	EXPECT_TRUE(bulges[0].isLine());
	EXPECT_TRUE(bulges[1].isLine());
	EXPECT_NEAR(QVector2D::dotProduct(strokeA, strokeB), 0.0f, 1e-4f);
	EXPECT_NEAR(strokeA.length(), lengths.a, 1e-5f);
	EXPECT_NEAR(strokeB.length(), lengths.b, 1e-5f);
	ExpectVectorNear(bulges[0].end(), bulges[1].start());
	// A runs along -side, arriving at B from the side.
	ExpectVectorNear(strokeA.normalized(), -candidate.side);
	ExpectVectorNear(bulges[1].end(), start + direction * bladeOffset);
	ExpectBulgeNear(bulges[2], start + direction * bladeOffset, start + startTangent * bladeOffset,
					std::tan(pi / 12.0f));
}

TEST(DragKnifeEntryTest, EntryWithoutSwivelEndsAlongStartTangent)
{
	const geometry::Bulge::List bulges = Bulges(geometry::tooloperation::DragKnifeEntry(xCandidate, lengths));
	ASSERT_EQ(bulges.size(), 2);
	ExpectBulgeNear(bulges[0], QVector2D(xStrokeBStart, lengths.a), QVector2D(xStrokeBStart, 0), 0.0f);
	ExpectBulgeNear(bulges[1], QVector2D(xStrokeBStart, 0), QVector2D(bladeOffset, 0), 0.0f);
}

TEST(DragKnifeEntryTest, ClearanceDetectsStrokeAZone)
{
	// Z_A spans one blade offset around A.
	EXPECT_FALSE(XCandidateClear(Segment(QVector2D(-6, 2), QVector2D(xStrokeBStart + 0.25f, 2)), false));
	EXPECT_TRUE(XCandidateClear(Segment(QVector2D(-6, 2), QVector2D(xStrokeBStart - 0.6f, 2)), false));
}

TEST(DragKnifeEntryTest, ClearanceDetectsStrokeBZone)
{
	EXPECT_FALSE(XCandidateClear(Segment(QVector2D(-2, 0.1f), QVector2D(-1.5f, 0.1f)), false));
	EXPECT_TRUE(XCandidateClear(Segment(QVector2D(-1.5f, 0.6f), QVector2D(-1, 0.6f)), false));
}

TEST(DragKnifeEntryTest, ClearanceDetectsStuckBladesZoneOnlyWhenEnabled)
{
	const geometry::Polyline belowB = Segment(QVector2D(-2, -0.1f), QVector2D(-1.5f, -0.1f));
	EXPECT_FALSE(XCandidateClear(belowB, true));
	EXPECT_TRUE(XCandidateClear(belowB, false));

	// A contour along B lies between the B and stuck blades zones.
	const geometry::Polyline alongB = Segment(QVector2D(-6, 0), QVector2D(0, 0));
	EXPECT_FALSE(XCandidateClear(alongB, true));
	EXPECT_TRUE(XCandidateClear(alongB, false));
}

TEST(DragKnifeEntryTest, ClearanceIgnoresContourTouchingStart)
{
	const geometry::Polyline corner({ geometry::Bulge(QVector2D(0, -10), QVector2D(0, 0), 0.0f),
									  geometry::Bulge(QVector2D(0, 0), QVector2D(10, 0), 0.0f) });
	EXPECT_TRUE(XCandidateClear(corner, true));
}

TEST(DragKnifeEntryTest, ConvexStartWithOutsideWasteNeedsNoSwivel)
{
	const geometry::tooloperation::DragKnifeEntryCandidate candidate = ExpectValidEntry(ccwSquare, false);
	EXPECT_EQ(candidate.swivel, 0.0f);
	ExpectVectorNear(candidate.side, QVector2D(0, -1));

	const geometry::tooloperation::ToolPolyline toolPolyline
		= GeneratedToolPolyline(ccwSquare, geometry::CuttingDirection::BACKWARD, geometry::CuttingDirection::BACKWARD);
	ASSERT_TRUE(toolPolyline.entry);
	const geometry::Bulge::List bulges = Bulges(*toolPolyline.entry);
	ASSERT_EQ(bulges.size(), 2);
	ExpectBulgeNear(bulges[0], QVector2D(xStrokeBStart, -lengths.a), QVector2D(xStrokeBStart, 0), 0.0f);
	ExpectBulgeNear(bulges[1], QVector2D(xStrokeBStart, 0), QVector2D(bladeOffset, 0), 0.0f);
}

TEST(DragKnifeEntryTest, SquareWithInsideWasteSwivelsFromInside)
{
	const geometry::tooloperation::DragKnifeEntryCandidate candidate = ExpectValidEntry(ccwSquare, true);
	// Waste wedge is the first quadrant, B can not follow the start tangent.
	EXPECT_GE(std::abs(candidate.swivel), pi / 2.0f - 1e-4f);
	EXPECT_LT(std::abs(candidate.swivel), pi);
	EXPECT_TRUE(geometry::tooloperation::DragKnifeEntryClear(ccwSquare, candidate, lengths, clearanceTolerance, true));
}

TEST(DragKnifeEntryTest, HexagonWithInsideWasteSwivelsFromInside)
{
	const geometry::tooloperation::DragKnifeEntryCandidate candidate = ExpectValidEntry(ccwHexagon, true);
	EXPECT_GE(std::abs(candidate.swivel), pi / 3.0f - 1e-4f);
	EXPECT_TRUE(geometry::tooloperation::DragKnifeEntryClear(ccwHexagon, candidate, lengths, clearanceTolerance, true));
}

TEST(DragKnifeEntryTest, CircleWithInsideWasteSwivelsFromInside)
{
	const geometry::tooloperation::DragKnifeEntryCandidate candidate = ExpectValidEntry(ccwCircle, true);
	EXPECT_GT(std::abs(candidate.swivel), 0.0f);
	EXPECT_TRUE(geometry::tooloperation::DragKnifeEntryClear(ccwCircle, candidate, lengths, clearanceTolerance, true));
}

TEST(DragKnifeEntryTest, CircleTravelledBackwardKeepsWasteSideOfPolyline)
{
	// Travel is clockwise, waste stays on the left of the polyline: inside the circle.
	const geometry::tooloperation::ToolPolyline toolPolyline
		= GeneratedToolPolyline(ccwCircle, geometry::CuttingDirection::FORWARD, geometry::CuttingDirection::BACKWARD);
	ASSERT_TRUE(toolPolyline.entry);

	const geometry::Bulge::List bulges = Bulges(*toolPolyline.entry);
	ASSERT_EQ(bulges.size(), 3);
	// Both strokes inside the circle.
	EXPECT_LT(bulges[0].start().length(), 10.0f);
	EXPECT_LT(bulges[1].start().length(), 10.0f);
}

TEST(DragKnifeEntryTest, OpenPolylineEntersAlongStartTangentFromWasteSide)
{
	for (const bool wasteOnLeft : { true, false }) {
		const geometry::tooloperation::DragKnifeEntryCandidate candidate = ExpectValidEntry(lPolyline, wasteOnLeft);
		// Nothing lies on the backward ray of an open start.
		EXPECT_EQ(candidate.swivel, 0.0f);
		ExpectVectorNear(candidate.side, QVector2D(0, wasteOnLeft ? 1 : -1));
	}
}

TEST(DragKnifeEntryTest, StraightClosedStartSwivelsAwayFromContour)
{
	// Starts in the middle of the bottom edge, B along it would put stuck blades on the part.
	const geometry::Polyline polyline
		= CcwPolygon({ QVector2D(5, 0), QVector2D(10, 0), QVector2D(10, 10), QVector2D(0, 10), QVector2D(0, 0) });
	const geometry::tooloperation::DragKnifeEntryCandidate candidate = ExpectValidEntry(polyline, false);
	EXPECT_NE(candidate.swivel, 0.0f);
	EXPECT_TRUE(geometry::tooloperation::DragKnifeEntryClear(polyline, candidate, lengths, clearanceTolerance, true));
}

TEST(DragKnifeEntryTest, ObstacleOnBackwardRayForcesSwivel)
{
	// Same start corner as the square, an arm of the part crosses the backward ray at x in [-2, -1].
	const geometry::Polyline polyline
		= CcwPolygon({ QVector2D(0, 0), QVector2D(10, 0), QVector2D(10, 10), QVector2D(-4, 10), QVector2D(-4, -6),
					   QVector2D(-2, -6), QVector2D(-2, -1), QVector2D(-1, -1), QVector2D(-1, 5), QVector2D(0, 5) });
	EXPECT_FALSE(geometry::tooloperation::DragKnifeEntryClear(
		polyline, { QVector2D(0, 0), QVector2D(1, 0), QVector2D(0, -1), 0.0f }, lengths, clearanceTolerance, false));

	const geometry::tooloperation::DragKnifeEntryCandidate candidate = ExpectValidEntry(polyline, false);
	EXPECT_NE(candidate.swivel, 0.0f);
	EXPECT_TRUE(geometry::tooloperation::DragKnifeEntryClear(polyline, candidate, lengths, clearanceTolerance, true));
}

TEST(DragKnifeEntryTest, FallbackIsFirstCandidateFromWaste)
{
	const std::optional<geometry::tooloperation::DragKnifeEntryCandidate> candidate
		= geometry::tooloperation::DragKnifeEntrySelect(tinyCcwSquare, true, lengths);
	ASSERT_TRUE(candidate);
	EXPECT_FALSE(
		geometry::tooloperation::DragKnifeEntryClear(tinyCcwSquare, *candidate, lengths, clearanceTolerance, false));
	// Smallest swivel within the waste wedge, B along the incoming edge.
	EXPECT_NEAR(std::abs(candidate->swivel), pi / 2.0f, 1e-4f);
}

TEST(DragKnifeEntryTest, EveryHeadingOutsideFlipToleranceAligns)
{
	for (const auto& [contour, wasteOnLeft] :
		 { std::pair { &ccwSquare, false }, std::pair { &ccwSquare, true }, std::pair { &lPolyline, true } }) {
		const std::optional<geometry::tooloperation::DragKnifeEntryCandidate> candidate
			= geometry::tooloperation::DragKnifeEntrySelect(*contour, wasteOnLeft, lengths);
		ASSERT_TRUE(candidate);

		const geometry::Polyline entry = geometry::tooloperation::DragKnifeEntry(*candidate, lengths);
		const geometry::tooloperation::DragKnifeTipZone zone(*candidate, lengths, false);

		float worst = 0.0f;
		constexpr int headingCount = 720;
		for (int i = 0; i < headingCount; ++i) {
			const float heading = 2.0f * pi * i / headingCount;
			if (std::abs(heading - pi) < flipTolerance) {
				continue;
			}

			const SimulationResult result = SimulateEntry(entry, zone, candidate->startTangent, heading);
			worst = std::max(worst, result.error);
			EXPECT_TRUE(result.tipsInZone) << heading;
		}
		EXPECT_LT(worst, alignmentTolerance + 0.05f * degree);
	}
}
