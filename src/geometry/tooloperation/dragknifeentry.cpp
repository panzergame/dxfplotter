module;

#include <algorithm>
#include <cmath>
#include <numbers>
#include <optional>
#include <utility>
#include <vector>

#include <QVector2D>

export module geometry.tooloperation.dragknifeentry;

import geometry.bulge;
import geometry.polyline;
import geometry.rect;
import geometry.utils;

/* L entry of a drag knife: stroke A, a sharp 90° corner, stroke B ending with the blade tip on the start, then a
 * swivel onto the start tangent. A trailing blade follows the tractrix tan(θ/2) = tan(θ₀/2)·e^(−s/r), θ = π being
 * an unstable equilibrium that a single stroke never leaves: the corner turns blades near π on A to ≈ π/2 on B.
 */

export namespace geometry::tooloperation
{

struct DragKnifeLengths
{
	float bladeOffset;
	// Stroke lengths.
	float a;
	float b;
	// Worst blade angle at the end of A, for initial headings outside π ± flip tolerance.
	float flipExit;
	// Abscissa on A where the worst heading crosses π/2.
	float a90;
	// Abscissa on B from which the worst tip comes back toward B's line.
	float c;
};

/** Minimise a + b: the worst headings π ± δ end A at ±x, the corner turns the worse to π/2 + x, B brings it down to
 * the alignment tolerance. The optimum is x = π/4.
 * @param flipTolerance δ, headings within π ± δ relative to A are not guaranteed to align.
 * @param alignmentTolerance Maximum blade angle from the start tangent at the end of the entry.
 */
DragKnifeLengths DragKnifeEntryLengths(float bladeOffset, float flipTolerance, float alignmentTolerance)
{
	const float silverRatio = 1.0f + std::numbers::sqrt2_v<float>;
	const float flipCotangent = 1.0f / std::tan(flipTolerance / 2.0f);

	const float a = std::max(0.0f, bladeOffset * std::log(silverRatio * flipCotangent));
	const float b = bladeOffset * std::log(silverRatio / std::tan(alignmentTolerance / 2.0f));
	const float flipExit = 2.0f * std::atan(flipCotangent * std::exp(-a / bladeOffset));
	const float a90 = bladeOffset * std::log(flipCotangent);
	const float c = bladeOffset * std::log(std::tan(M_PI / 4.0f + flipExit / 2.0f));

	return { bladeOffset, a, b, flipExit, a90, c };
}

struct DragKnifeEntryCandidate
{
	// Contour start and its tangent.
	QVector2D start;
	QVector2D startTangent;
	// Side of B's line where stroke A comes from.
	QVector2D side;
	// SignedAngle(direction(), startTangent), the swivel around the start.
	float swivel;

	QVector2D direction() const { return RotateVector(startTangent, -swivel); }
};

/** Area swept by the blade tip during strokes A and B, for every initial heading outside π ± flip tolerance, and
 * optionally for blades stuck near π by friction. The swivel keeps the tip on the start and adds nothing.
 * Coordinates are taken in B's frame: origin at B's start, u along B, v along the side A comes from.
 */
class DragKnifeTipZone
{
private:
	QVector2D m_origin;
	QVector2D m_u;
	QVector2D m_v;
	DragKnifeLengths m_lengths;
	bool m_stuckBlades;

	float trailWidth(float travelled, float straightUntil) const
	{
		const float r = m_lengths.bladeOffset;
		return (travelled <= straightUntil) ? r : r * Sech((travelled - straightUntil) / r);
	}

	float strokeBEnd() const { return m_lengths.b - m_lengths.bladeOffset; }

	bool inStrokeA(float u, float v) const
	{
		const float r = m_lengths.bladeOffset;
		const float travelled = m_lengths.a - v;
		return -r <= travelled && travelled <= m_lengths.a - r * std::cos(m_lengths.flipExit)
			&& std::abs(u) <= trailWidth(travelled, m_lengths.a90);
	}

	bool inStrokeBOnASide(float u, float v) const
	{
		return -m_lengths.bladeOffset * std::sin(m_lengths.flipExit) <= u && u <= strokeBEnd() && 0.0f <= v
			&& v <= trailWidth(u, m_lengths.c);
	}

	/* Stuck blades lead the holder along A, reach the corner at v = -r and converge on the opposite side. The
	 * part of their path on A, u = 0 for v in [-r, a - r], is already covered by the A and B areas and this one.
	 */
	bool inStuckBladesConvergence(float u, float v) const
	{
		return 0.0f <= u && u <= strokeBEnd() && v <= 0.0f && v >= -trailWidth(u, 0.0f);
	}

	bool containsLocal(float u, float v) const
	{
		return inStrokeA(u, v) || inStrokeBOnASide(u, v) || (m_stuckBlades && inStuckBladesConvergence(u, v));
	}

public:
	explicit DragKnifeTipZone(const DragKnifeEntryCandidate& candidate, const DragKnifeLengths& lengths,
							  bool stuckBlades)
		: m_origin(candidate.start + candidate.direction() * (lengths.bladeOffset - lengths.b))
		, m_u(candidate.direction())
		, m_v(candidate.side)
		, m_lengths(lengths)
		, m_stuckBlades(stuckBlades)
	{
	}

	bool contains(const QVector2D& point) const
	{
		const QVector2D relative = point - m_origin;
		return containsLocal(QVector2D::dotProduct(relative, m_u), QVector2D::dotProduct(relative, m_v));
	}

	/// Point deeper than depth inside the zone, approximated by a ring of probes around it.
	bool containsDeeper(const QVector2D& point, float depth) const
	{
		static constexpr int probeCount = 16;

		if (!contains(point)) {
			return false;
		}

		for (int i = 0; i < probeCount; ++i) {
			const float angle = 2.0f * M_PI * i / probeCount;
			if (!contains(point + QVector2D(std::cos(angle), std::sin(angle)) * depth)) {
				return false;
			}
		}

		return true;
	}

	Rect boundingRect() const
	{
		const float r = m_lengths.bladeOffset;
		const QVector2D uMin = m_u * -r;
		const QVector2D uMax = m_u * (m_lengths.b - r);
		const QVector2D vMin = m_v * -r;
		const QVector2D vMax = m_v * (m_lengths.a + r);

		// The frame is rotated, both diagonals are needed to bound the four corners.
		return Rect(m_origin + uMin + vMin, m_origin + uMax + vMax)
			| Rect(m_origin + uMax + vMin, m_origin + uMin + vMax);
	}
};

/** True when the contour does not enter the tip zone deeper than tolerance. The contour reaches the zone at the
 * start by construction, there the zone is thinner than the tolerance.
 */
bool DragKnifeEntryClear(const Polyline& contour, const DragKnifeEntryCandidate& candidate,
						 const DragKnifeLengths& lengths, float tolerance, bool stuckBlades)
{
	const DragKnifeTipZone zone(candidate, lengths, stuckBlades);
	const Rect zoneRect = zone.boundingRect();

	// A sample misses the deepest intrusion by at most half a step.
	const float step = tolerance / 4.0f;

	return !contour.anyBulge([&](const Bulge& bulge) {
		return !bulge.isPoint() && bulge.boundingRect().intersects(zoneRect)
			&& bulge.anySample(step, [&](const QVector2D& point) {
				   return zone.containsDeeper(point, tolerance);
			   });
	});
}

}

namespace geometry::tooloperation
{

constexpr float sectorFuzz = 1e-4f;

/* Waste wedge, swept from the start tangent toward the waste up to the incoming edge at a closed start, the
 * waste half plane at an open start.
 */
float WasteWedge(const Polyline& contour, float towardWaste)
{
	if (!contour.isClosed()) {
		return M_PI;
	}

	const float angle = towardWaste * SignedAngle(contour.startTangent(), -contour.endTangent());
	const float wedge = (angle < -sectorFuzz) ? angle + 2.0f * M_PI : angle;
	return (wedge <= sectorFuzz) ? 2.0f * M_PI : wedge;
}

/* The backward direction of B is the backward start tangent rotated toward the waste by an angle, which is also
 * the swivel magnitude, from the start tangent it lies at pi - angle.
 */
std::vector<float> SwivelAngles(float wedge)
{
	static constexpr float gridStep = M_PI / 36.0f;

	std::vector<float> angles { 0.0f };
	for (int i = 1; i * gridStep < M_PI - sectorFuzz; ++i) {
		angles.push_back(i * gridStep);
		angles.push_back(-i * gridStep);
	}
	// A wedge narrower than the grid step still holds its bisector.
	angles.push_back(M_PI - wedge / 2.0f);

	return angles;
}

std::vector<DragKnifeEntryCandidate> DragKnifeEntryCandidates(const QVector2D& start, const QVector2D& startTangent,
															  float towardWaste, float wedge, bool closed)
{
	const QVector2D waste = towardWaste * PerpendicularLine(startTangent);

	std::vector<DragKnifeEntryCandidate> candidates;
	for (const float angle : SwivelAngles(wedge)) {
		const float fromTangent = M_PI - angle;
		const bool inSector = fromTangent >= -sectorFuzz && fromTangent <= wedge + sectorFuzz;
		if (!inSector) {
			continue;
		}

		const float swivel = towardWaste * angle;
		const QVector2D side = PerpendicularLine(RotateVector(startTangent, -swivel));
		const auto addCandidate = [&](const QVector2D& candidateSide) {
			candidates.push_back({ start, startTangent, candidateSide, swivel });
		};

		/* Along a contour edge the zone lies on one side only, clearance can not tell whether it is the part, so A
		 * must come from inside the wedge.
		 */
		const bool alongFirstEdge = fromTangent <= sectorFuzz;
		const bool alongLastEdge = closed && fromTangent >= wedge - sectorFuzz;
		if (alongFirstEdge) {
			addCandidate(-towardWaste * side);
		} else if (alongLastEdge) {
			addCandidate(towardWaste * side);
		} else {
			const QVector2D preferredSide = (QVector2D::dotProduct(side, waste) >= 0.0f) ? side : -side;
			addCandidate(preferredSide);
			addCandidate(-preferredSide);
		}
	}

	return candidates;
}

std::optional<DragKnifeEntryCandidate> FirstClearCandidate(const Polyline& contour,
														   const std::vector<DragKnifeEntryCandidate>& candidates,
														   const DragKnifeLengths& lengths, float tolerance,
														   bool stuckBlades)
{
	const auto it = std::ranges::find_if(candidates, [&](const DragKnifeEntryCandidate& candidate) {
		return DragKnifeEntryClear(contour, candidate, lengths, tolerance, stuckBlades);
	});

	return (it != candidates.end()) ? std::make_optional(*it) : std::nullopt;
}

}

export namespace geometry::tooloperation
{

/** Pick the entry needing the smallest swivel whose B arrives from the waste side and whose tip zone stays clear of
 * the contour. Falls back to ignoring stuck blades, then to the first candidate from the waste side.
 * @param contour Polyline in travel direction, its tip path.
 * @param wasteOnLeft Side of the travel direction where the knife may mark.
 */
std::optional<DragKnifeEntryCandidate> DragKnifeEntrySelect(const Polyline& contour, bool wasteOnLeft,
															const DragKnifeLengths& lengths)
{
	static constexpr float toleranceRatio = 0.05f;

	if (contour.isPoint()) {
		return std::nullopt;
	}

	// Rotation sign turning the start tangent toward the waste.
	const float towardWaste = wasteOnLeft ? 1.0f : -1.0f;
	const float wedge = WasteWedge(contour, towardWaste);
	const std::vector<DragKnifeEntryCandidate> candidates
		= DragKnifeEntryCandidates(contour.start(), contour.startTangent(), towardWaste, wedge, contour.isClosed());
	if (candidates.empty()) {
		return std::nullopt;
	}

	const float tolerance = toleranceRatio * lengths.bladeOffset;
	return FirstClearCandidate(contour, candidates, lengths, tolerance, true)
		.or_else([&] {
			return FirstClearCandidate(contour, candidates, lengths, tolerance, false);
		})
		.value_or(candidates.front());
}

/// Holder path: stroke A, stroke B ending with the tip on the start, swivel onto the start tangent.
Polyline DragKnifeEntry(const DragKnifeEntryCandidate& candidate, const DragKnifeLengths& lengths)
{
	const QVector2D direction = candidate.direction();
	const QVector2D bEnd = candidate.start + direction * lengths.bladeOffset;
	const QVector2D bStart = bEnd - direction * lengths.b;
	const QVector2D aStart = bStart + candidate.side * lengths.a;

	Bulge::List bulges { Bulge(aStart, bStart, 0.0f), Bulge(bStart, bEnd, 0.0f) };
	if (candidate.swivel != 0.0f) {
		bulges.push_back(
			Bulge::swivel(candidate.start, direction, candidate.startTangent, candidate.swivel, lengths.bladeOffset));
	}

	return Polyline(std::move(bulges));
}

}
