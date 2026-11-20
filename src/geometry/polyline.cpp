module;

#include <algorithm>
#include <cmath>
#include <optional>
#include <vector>
#include <geometry/cavcutils.h>
#include <cavc/polylineoffset.hpp>
#include <QVector2D>
#include <QTransform>

export module geometry.polyline;

import common.aggregable;
import serializer.access;
import geometry.bulge;
import geometry.rect;
import geometry.utils;

namespace geometry
{

inline float winding(const QVector2D& start, const QVector2D& end)
{
	return (end.x() - start.x()) * (end.y() + start.y());
}

inline float winding(const Bulge& bulge)
{
	return winding(bulge.start(), bulge.end());
}

// Holder arc of a drag knife pivoting around its blade tip at vertex.
inline Bulge swivelBulge(const QVector2D& vertex, const QVector2D& fromTangent, const QVector2D& toTangent,
						 float turn, float bladeOffset)
{
	return Bulge(vertex + fromTangent * bladeOffset, vertex + toTangent * bladeOffset, std::tan(turn / 4.0f));
}

/** Direction of a drag knife alignment stroke ending at a closed polyline start, kept on the waste side.
 * The backward extension of the first tangent lies outside the start turn, outside the incoming arc at a smooth
 * start, and on the contour itself after a straight smooth start.
 */
inline QVector2D closedAlignmentTangent(const Bulge& last, const QVector2D& firstTangent, bool wasteOnLeft)
{
	static constexpr float smoothTurn = 1e-3f;

	const QVector2D lastTangent = last.endTangent();
	const float turn = SignedAngle(lastTangent, firstTangent);
	const bool smooth = std::abs(turn) <= smoothTurn;
	const float bend = smooth ? last.tangent() : turn;
	if (bend == 0.0f || (bend < 0.0f) == wasteOnLeft) {
		return firstTangent;
	}

	// A smooth start has no corner bisector, the waste side normal takes its place.
	const QVector2D leftNormal = PerpendicularLine(firstTangent);
	const QVector2D wasteDirection
		= smooth ? (wasteOnLeft ? leftNormal : -leftNormal) : (firstTangent - lastTangent).normalized();
	return -wasteDirection;
}

}

export namespace geometry
{

class Polyline : public common::Aggregable<Polyline>
{
	friend serializer::Access<Polyline>;

private:
	Bulge::List m_bulges;

public:
	/* cavc interop, consumed by geometry.pocketer. Public rather than a friend declaration:
	 * befriending Pocketer here would declare it in this module and clash with its definition
	 * in geometry.pocketer.
	 */
	explicit Polyline(const cavc::Polyline<double>& polyline)
	{
		m_bulges.resize(polyline.isClosed() ? polyline.size() : polyline.size() - 1);

		polyline.visitSegIndices([&polyline, this](size_t i, size_t j) {
			m_bulges[i] = Bulge(polyline[i], polyline[j]);
			return true;
		});
	}

	cavc::Polyline<double> toCavc() const
	{
		cavc::Polyline<double> ccPolyline;

		// Convert to CAVC polyline
		forEachBulge([&ccPolyline](const Bulge& bulge) {
			const QVector2D& start = bulge.start();
			ccPolyline.addVertex(start.x(), start.y(), bulge.tangent());
		});

		const bool closed = isClosed();
		if (!closed) {
			const QVector2D& endV = end();
			ccPolyline.addVertex(endV.x(), endV.y(), 0.0f);
		}

		ccPolyline.isClosed() = closed;

		return ccPolyline;
	}

	cavc::Polyline<double> toCavc(Orientation expectedOrientation) const
	{
		if (orientation() != expectedOrientation) {
			return inverse().toCavc();
		}
		return toCavc();
	}

	cavc::Polyline<double> toCavc(bool inverse) const;

	explicit Polyline() = default;
	explicit Polyline(Bulge::List&& bulges)
		: m_bulges(bulges)
	{
		assert(!m_bulges.empty());
	}

	const QVector2D& start() const
	{
		assert(!m_bulges.empty());

		return m_bulges.front().start();
	}

	QVector2D& start()
	{
		assert(!m_bulges.empty());

		return m_bulges.front().start();
	}

	const QVector2D& end() const
	{
		assert(!m_bulges.empty());

		return m_bulges.back().end();
	}

	QVector2D& end()
	{
		assert(!m_bulges.empty());

		return m_bulges.back().end();
	}

	bool isClosed() const
	{
		assert(!m_bulges.empty());

		return (start() == end());
	}

	bool isPoint() const
	{
		assert(!m_bulges.empty());

		return isClosed() && (m_bulges.size() == 1);
	}

	bool isLine() const
	{
		assert(!m_bulges.empty());

		return (m_bulges.size() == 1) && m_bulges.front().isLine();
	}

	float length() const
	{
		assert(!m_bulges.empty());

		return std::accumulate(m_bulges.begin(), m_bulges.end(), 0.0f, [](float sum, const Bulge& bulge) {
			return sum + bulge.length();
		});
	}

	Orientation orientation() const
	{
		assert(!m_bulges.empty() && isClosed());

		const float windingSum
			= std::accumulate(m_bulges.begin(), m_bulges.end(), 0.0f, [](float sum, const Bulge& bulge) {
				  return sum + winding(bulge);
			  });

		return (windingSum > 0) ? Orientation::CW : Orientation::CCW;
	}

	Rect boundingRect() const
	{
		Bulge::List::const_iterator it = m_bulges.begin();
		const Rect firstBoundingRest = (it++)->boundingRect();

		return std::transform_reduce(it, m_bulges.end(), firstBoundingRest, std::bit_or(), [](const Bulge& bulge) {
			return bulge.boundingRect();
		});
	}

	Polyline& invert()
	{
		for (Bulge& bulge : m_bulges) {
			bulge.invert();
		}

		std::reverse(m_bulges.begin(), m_bulges.end());

		return *this;
	}

	Polyline inverse() const
	{
		Polyline inversed = *this;
		return inversed.invert();
	}

	Polyline& operator+=(const Polyline& other)
	{
		m_bulges.insert(m_bulges.end(), other.m_bulges.begin(), other.m_bulges.end());

		return *this;
	}

	template<class Functor>
	void forEachBulge(Functor&& functor) const
	{
		for (const Bulge& bulge : m_bulges) {
			functor(bulge);
		}
	}

	template<class Functor>
	void transformBulge(Functor&& functor)
	{
		for (Bulge& bulge : m_bulges) {
			functor(bulge);
		}
	}

	Polyline::List offsetted(float margin) const
	{
		if (isPoint()) {
			return { *this };
		}

		// Offset CAVC polyline
		std::vector<cavc::Polyline<double>> offsettedCcPolylines = cavc::parallelOffset(toCavc(), (double)margin);

		// Convert back to polylines
		Polyline::List offsettedPolylines(offsettedCcPolylines.size());
		std::transform(offsettedCcPolylines.begin(), offsettedCcPolylines.end(), offsettedPolylines.begin(),
					   [](const cavc::Polyline<double>& polyline) {
						   return Polyline(polyline);
					   });

		return offsettedPolylines;
	}

	/** Holder path of a drag knife whose blade tip trails by bladeOffset: shift every bulge forward along its
	 * tangent so the tip follows this polyline, and pivot the holder around each vertex where the tangent breaks.
	 */
	Polyline dragKnifeCompensated(float bladeOffset, float minimumSwivelAngle) const
	{
		Bulge::List bulges;
		std::optional<QVector2D> previousTangent;
		std::optional<QVector2D> firstTangent;

		// Below the minimum swivel angle the gap is bladeOffset * angle, closed by moving the next bulge start.
		const auto joint = [&](const QVector2D& vertex, const QVector2D& fromTangent, const QVector2D& toTangent) {
			const float turn = SignedAngle(fromTangent, toTangent);
			if (std::abs(turn) > minimumSwivelAngle) {
				bulges.push_back(swivelBulge(vertex, fromTangent, toTangent, turn, bladeOffset));
			}
		};

		forEachBulge([&](const Bulge& bulge) {
			// Zero length bulges have no tangent.
			if (bulge.start() == bulge.end()) {
				return;
			}

			const QVector2D startTangent = bulge.startTangent();
			const QVector2D endTangent = bulge.endTangent();

			if (previousTangent) {
				joint(bulge.start(), *previousTangent, startTangent);
			} else {
				firstTangent = startTangent;
			}

			// Arc keeps its center and sweep angle, so its bulge tangent is unchanged.
			Bulge shifted(bulge.start() + startTangent * bladeOffset, bulge.end() + endTangent * bladeOffset,
						  bulge.tangent());
			if (!bulges.empty()) {
				shifted.start() = bulges.back().end();
			}
			bulges.push_back(shifted);

			previousTangent = endTangent;
		});

		if (bulges.empty()) {
			return *this;
		}

		if (isClosed()) {
			joint(start(), *previousTangent, *firstTangent);
			bulges.front().start() = bulges.back().end();
		}

		return Polyline(std::move(bulges));
	}

	/** Holder entry ending where dragKnifeCompensated() starts. A straight stroke settles a blade of unknown
	 * heading onto its direction, as a trailing blade converges to the travel direction. When the stroke can not
	 * follow the first tangent, a swivel around the start follows, the blade heading being known by then.
	 * @param wasteOnLeft Side of the travel direction where a closed polyline stroke may cut.
	 */
	std::optional<Polyline> dragKnifeAlignment(float bladeOffset, float alignmentLength, bool wasteOnLeft) const
	{
		if (alignmentLength <= 0.0f) {
			return std::nullopt;
		}

		// Same zero length bulge skipping as dragKnifeCompensated() to find the tangents.
		const auto hasLength = [](const Bulge& bulge) {
			return bulge.start() != bulge.end();
		};
		const Bulge::List::const_iterator first = std::find_if(m_bulges.begin(), m_bulges.end(), hasLength);
		if (first == m_bulges.end()) {
			return std::nullopt;
		}
		const Bulge& last = *std::find_if(m_bulges.rbegin(), m_bulges.rend(), hasLength);

		const QVector2D& start = first->start();
		const QVector2D firstTangent = first->startTangent();
		const QVector2D strokeTangent
			= isClosed() ? closedAlignmentTangent(last, firstTangent, wasteOnLeft) : firstTangent;

		const QVector2D strokeEnd = start + strokeTangent * bladeOffset;
		Bulge::List bulges { Bulge(strokeEnd - strokeTangent * alignmentLength, strokeEnd, 0.0f) };

		if (strokeTangent != firstTangent) {
			bulges.push_back(swivelBulge(start, strokeTangent, firstTangent, SignedAngle(strokeTangent, firstTangent),
										 bladeOffset));
		}

		return Polyline(std::move(bulges));
	}

	void transform(const QTransform& matrix)
	{
		transformBulge([&matrix](Bulge& bulge) {
			bulge.transform(matrix);
		});
	}

	bool operator==(const Polyline& other) const { return m_bulges == other.m_bulges; }

	bool equals(const Polyline& other, bool inverse) const
	{
		if (inverse) {
			return std::equal(m_bulges.begin(), m_bulges.end(), other.m_bulges.rbegin(), other.m_bulges.rend(),
							  [](const Bulge& b1, const Bulge& b2) {
								  return b1.equalsInversed(b2);
							  });
		}

		return *this == other;
	}
};

}
