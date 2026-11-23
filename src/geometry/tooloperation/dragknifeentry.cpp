module;

#include <algorithm>
#include <array>
#include <cmath>
#include <numbers>
#include <optional>
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

namespace geometry::tooloperation
{

float Sech(float x)
{
	return 1.0f / std::cosh(x);
}

}

export namespace geometry::tooloperation
{

struct DragKnifeLengths
{
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
	const float c = bladeOffset * std::log(std::tan(std::numbers::pi_v<float> / 4.0f + flipExit / 2.0f));

	return { a, b, flipExit, a90, c };
}

struct DragKnifeEntryCandidate
{
	// Contour start and its tangent.
	QVector2D start;
	QVector2D startTangent;
	// Direction of stroke B.
	QVector2D direction;
	// Side of B's line where stroke A comes from.
	QVector2D side;
	// SignedAngle(direction, startTangent), the swivel around the start.
	float swivel;
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
	float m_bladeOffset;
	bool m_stuckBlades;

	bool containsLocal(float u, float v) const
	{
		const float r = m_bladeOffset;

		// Stroke A, a being the distance travelled on A.
		const float a = m_lengths.a - v;
		if (-r <= a && a <= m_lengths.a - r * std::cos(m_lengths.flipExit)) {
			const float width = (a <= m_lengths.a90) ? r : r * Sech((a - m_lengths.a90) / r);
			if (std::abs(u) <= width) {
				return true;
			}
		}

		const float uEnd = m_lengths.b - r;

		// Stroke B, on A's side only.
		if (-r * std::sin(m_lengths.flipExit) <= u && u <= uEnd && 0.0f <= v) {
			const float height = (u <= m_lengths.c) ? r : r * Sech((u - m_lengths.c) / r);
			if (v <= height) {
				return true;
			}
		}

		/* Stuck blades lead the holder along A, reach the corner at v = -r and converge on the opposite side. The
		 * part of their path on A, u = 0 for v in [-r, a - r], is already covered by the A, B and this area.
		 */
		if (m_stuckBlades && 0.0f <= u && u <= uEnd && v <= 0.0f) {
			return v >= -r * Sech(u / r);
		}

		return false;
	}

public:
	explicit DragKnifeTipZone(const DragKnifeEntryCandidate& candidate, const DragKnifeLengths& lengths,
							  float bladeOffset, bool stuckBlades)
		: m_origin(candidate.start + candidate.direction * (bladeOffset - lengths.b))
		, m_u(candidate.direction)
		, m_v(candidate.side)
		, m_lengths(lengths)
		, m_bladeOffset(bladeOffset)
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
			const float angle = 2.0f * std::numbers::pi_v<float> * i / probeCount;
			if (!contains(point + QVector2D(std::cos(angle), std::sin(angle)) * depth)) {
				return false;
			}
		}

		return true;
	}

	Rect boundingRect() const
	{
		const float r = m_bladeOffset;
		const std::array<QVector2D, 4> corners
			= { m_origin + m_u * -r + m_v * -r, m_origin + m_u * (m_lengths.b - r) + m_v * -r,
				m_origin + m_u * -r + m_v * (m_lengths.a + r),
				m_origin + m_u * (m_lengths.b - r) + m_v * (m_lengths.a + r) };

		Rect rect(corners[0]);
		for (const QVector2D& corner : corners) {
			rect |= Rect(corner);
		}
		return rect;
	}
};

/** True when the contour does not enter the tip zone deeper than tolerance. The contour reaches the zone at the
 * start by construction, there the zone is thinner than the tolerance.
 */
bool DragKnifeEntryClear(const Polyline& contour, const DragKnifeEntryCandidate& candidate,
						 const DragKnifeLengths& lengths, float bladeOffset, float tolerance, bool stuckBlades)
{
	const DragKnifeTipZone zone(candidate, lengths, bladeOffset, stuckBlades);
	const Rect zoneRect = zone.boundingRect();

	// A sample misses the deepest intrusion by at most half a step.
	const float step = tolerance / 4.0f;

	bool clear = true;
	contour.forEachBulge([&](const Bulge& bulge) {
		if (!clear || bulge.start() == bulge.end()) {
			return;
		}

		const Rect rect = bulge.boundingRect();
		if (rect.max().x() < zoneRect.min().x() || rect.min().x() > zoneRect.max().x()
			|| rect.max().y() < zoneRect.min().y() || rect.min().y() > zoneRect.max().y()) {
			return;
		}

		// Point at arc length s: start + sin(ks)/k·T + (1 − cos(ks))/k·N, k the signed curvature.
		const QVector2D startTangent = bulge.startTangent();
		const QVector2D normal = PerpendicularLine(startTangent);
		const float chord = bulge.start().distanceToPoint(bulge.end());
		const float sweep = 4.0f * std::atan(bulge.tangent());
		const bool line = bulge.isLine();
		const float curvature = line ? 0.0f : 2.0f * std::sin(sweep / 2.0f) / chord;
		const float length = line ? chord : sweep / curvature;

		const int count = std::max(1, int(std::ceil(length / step)));
		for (int i = 0; i <= count && clear; ++i) {
			const float s = length * i / count;
			const QVector2D point = line ? bulge.start() + startTangent * s
										 : bulge.start() + startTangent * (std::sin(curvature * s) / curvature)
					+ normal * ((1.0f - std::cos(curvature * s)) / curvature);
			clear = !zone.containsDeeper(point, tolerance);
		}
	});

	return clear;
}

/** Pick the entry needing the smallest swivel whose B arrives from the waste side and whose tip zone stays clear of
 * the contour. Falls back to ignoring stuck blades, then to the first candidate from the waste side.
 * @param contour Polyline in travel direction, its tip path.
 * @param wasteOnLeft Side of the travel direction where the knife may mark.
 */
std::optional<DragKnifeEntryCandidate> DragKnifeEntrySelect(const Polyline& contour, bool wasteOnLeft,
															const DragKnifeLengths& lengths, float bladeOffset)
{
	static constexpr float gridStep = std::numbers::pi_v<float> / 36.0f;
	static constexpr float sectorFuzz = 1e-4f;
	static constexpr float toleranceRatio = 0.05f;

	// Same zero length bulge skipping as dragKnifeCompensated() to find the tangents.
	std::optional<Bulge> first;
	std::optional<Bulge> last;
	contour.forEachBulge([&](const Bulge& bulge) {
		if (bulge.start() != bulge.end()) {
			if (!first) {
				first = bulge;
			}
			last = bulge;
		}
	});
	if (!first) {
		return std::nullopt;
	}

	const QVector2D start = first->start();
	const QVector2D startTangent = first->startTangent();
	const QVector2D leftNormal = PerpendicularLine(startTangent);
	const QVector2D waste = wasteOnLeft ? leftNormal : -leftNormal;
	// Rotation sign turning the start tangent toward the waste.
	const float towardWaste = wasteOnLeft ? 1.0f : -1.0f;

	constexpr float pi = std::numbers::pi_v<float>;
	const auto wasteAngle = [&](const QVector2D& vector) {
		const float angle = towardWaste * SignedAngle(startTangent, vector);
		return (angle < -sectorFuzz) ? angle + 2.0f * pi : angle;
	};

	// Waste wedge at a closed start, swept from the start tangent toward the waste up to the incoming edge.
	const bool closed = contour.isClosed();
	float wedge = closed ? wasteAngle(-last->endTangent()) : pi;
	if (wedge <= sectorFuzz) {
		wedge = 2.0f * pi;
	}

	/* The backward direction of B is the backward start tangent rotated toward the waste by an angle, which is also
	 * the swivel magnitude, from the start tangent it lies at pi - angle.
	 */
	std::vector<float> angles { 0.0f };
	for (int i = 1; i * gridStep < pi - sectorFuzz; ++i) {
		angles.push_back(i * gridStep);
		angles.push_back(-i * gridStep);
	}
	// A wedge narrower than the grid step still holds its bisector.
	angles.push_back(pi - wedge / 2.0f);

	std::vector<DragKnifeEntryCandidate> candidates;
	for (const float angle : angles) {
		const float fromTangent = pi - angle;
		const bool inSector = closed ? (fromTangent >= -sectorFuzz && fromTangent <= wedge + sectorFuzz)
									 : (fromTangent >= -sectorFuzz && fromTangent <= pi + sectorFuzz);
		if (!inSector) {
			continue;
		}

		// Exact at no swivel so that B ends where the compensated polyline starts.
		const QVector2D direction
			= (angle == 0.0f) ? startTangent : -RotateVector(startTangent, towardWaste * fromTangent);
		const float swivel = (angle == 0.0f) ? 0.0f : SignedAngle(direction, startTangent);

		/* Along a contour edge the zone lies on one side only, clearance can not tell whether it is the part, so A
		 * must come from inside the wedge.
		 */
		const bool alongFirstEdge = fromTangent <= sectorFuzz;
		const bool alongLastEdge = closed && fromTangent >= wedge - sectorFuzz;
		const QVector2D backward = -direction;
		if (alongFirstEdge) {
			candidates.push_back(
				{ start, startTangent, direction, RotateVector(backward, towardWaste * pi / 2.0f), swivel });
		} else if (alongLastEdge) {
			candidates.push_back(
				{ start, startTangent, direction, RotateVector(backward, -towardWaste * pi / 2.0f), swivel });
		} else {
			const QVector2D side = PerpendicularLine(direction);
			const QVector2D preferredSide = (QVector2D::dotProduct(side, waste) >= 0.0f) ? side : -side;
			candidates.push_back({ start, startTangent, direction, preferredSide, swivel });
			candidates.push_back({ start, startTangent, direction, -preferredSide, swivel });
		}
	}

	if (candidates.empty()) {
		return std::nullopt;
	}

	const float tolerance = toleranceRatio * bladeOffset;
	for (const bool stuckBlades : { true, false }) {
		for (const DragKnifeEntryCandidate& candidate : candidates) {
			if (DragKnifeEntryClear(contour, candidate, lengths, bladeOffset, tolerance, stuckBlades)) {
				return candidate;
			}
		}
	}

	return candidates.front();
}

/// Holder path: stroke A, stroke B ending with the tip on the start, swivel onto the start tangent.
Polyline DragKnifeEntry(const DragKnifeEntryCandidate& candidate, const DragKnifeLengths& lengths, float bladeOffset)
{
	const QVector2D bEnd = candidate.start + candidate.direction * bladeOffset;
	const QVector2D bStart = bEnd - candidate.direction * lengths.b;
	const QVector2D aStart = bStart + candidate.side * lengths.a;

	Bulge::List bulges { Bulge(aStart, bStart, 0.0f), Bulge(bStart, bEnd, 0.0f) };
	if (candidate.swivel != 0.0f) {
		bulges.push_back(
			Bulge::swivel(candidate.start, candidate.direction, candidate.startTangent, candidate.swivel, bladeOffset));
	}

	return Polyline(std::move(bulges));
}

}
