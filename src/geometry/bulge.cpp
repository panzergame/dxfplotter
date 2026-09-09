module;

#include <cmath>
#include <iomanip>
#include <iterator>
#include <limits>
#include <geometry/cavcutils.h>
#include <cavc/plinesegment.hpp>
#include <serializer/access.h>
#include <QVector2D>
#include <QTransform>

export module geometry.bulge;

import common.aggregable;
import geometry.arc;
import geometry.circle;
import geometry.line;
import geometry.rect;
import geometry.utils;

namespace geometry
{

Rect boundingRectPoints(const Point2DList& points)
{
	Point2DList::const_iterator it = points.begin();
	const QVector2D firstPoint = *(it++);

	return std::accumulate(it, points.end(), Rect(firstPoint));
}

inline QVector2D mapVector2D(const QVector2D& vect, const QTransform& matrix)
{
	const QPointF point = vect.toPointF();
	return QVector2D(matrix.map(point));
}

}

export namespace geometry
{

class Bulge : public common::Aggregable<Bulge>
{
	friend serializer::Access<Bulge>;

private:
	QVector2D m_start;
	QVector2D m_end;

	float m_tangent;

	float arcRadius() const
	{
		const float absTangent = std::abs(m_tangent);

		const QVector2D line = m_end - m_start;
		const float lineLength = line.length();
		const float radius = (lineLength * (1.0f + m_tangent * m_tangent)) / (4.0f * absTangent);

		return radius;
	}

	QVector2D relativeArcCenter() const
	{
		const QVector2D line = m_end - m_start;
		const QVector2D halfLine = line / 2.0f;

		const float theta4 = std::atan(m_tangent);
		const float lineToCenterAngle = M_PI_2 - theta4 * 2.0f;

		if (lineToCenterAngle < precision::fuzzyAngle) {
			return halfLine;
		} else {
			const float lineToCenterTangent = std::tan(lineToCenterAngle);

			const QVector2D perpendicularHalfLine(-halfLine.y(), halfLine.x());
			const QVector2D relativeCenter = halfLine + perpendicularHalfLine * lineToCenterTangent;
			return relativeCenter;
		}
	}

	Point2DList arcBoundingPoints() const
	{
		using QVector2D4 = std::array<QVector2D, 4>;

		Point2DList points;

		const QVector2D relativeCenter = relativeArcCenter();
		const QVector2D line = m_end - m_start;
		const float radius = arcRadius();

		static const QVector2D4 quadrantPointsUnit
			= { QVector2D(1, 0), QVector2D(0, 1), QVector2D(-1, 0), QVector2D(0, -1) };

		const bool centerLineSide = std::signbit(CrossProduct(line, relativeCenter));

		QVector2D4 quadrantPoints;
		std::transform(quadrantPointsUnit.begin(), quadrantPointsUnit.end(), quadrantPoints.begin(),
					   [&relativeCenter, radius](const QVector2D& pointUnit) {
						   return relativeCenter + pointUnit * radius;
					   });

		std::copy_if(quadrantPoints.begin(), quadrantPoints.end(), std::back_inserter(points),
					 [centerLineSide, &line](const QVector2D& point) {
						 const bool pointLineSide = std::signbit(CrossProduct(line, point));
						 const bool oppositeSideToCenter = (pointLineSide != centerLineSide);

						 return oppositeSideToCenter;
					 });

		std::transform(points.begin(), points.end(), points.begin(), [this](const QVector2D& point) {
			return point + m_start;
		});

		return points;
	}

public:
	/** Define a bulge
	 * @param start Starting point of the bulge
	 * @param end Ending point of the bulge
	 * @param tangent Tangent of a thourth of the arc angle.
	 * Negative tangent means the arc goes clockwise from start to end,
	 * otherwise anti clockwise from start to end.
	 */
	explicit Bulge(const QVector2D& start, const QVector2D& end, float tangent)
		: m_start(start)
		, m_end(end)
		, m_tangent(tangent)
	{
		assert(-1.0f <= tangent && tangent <= 1.0f);
	}

	explicit Bulge(const cavc::PlineVertex<double>& v1, const cavc::PlineVertex<double>& v2)
		: m_start(v1.x(), v1.y())
		, m_end(v2.x(), v2.y())
		, m_tangent(v1.bulge())
	{
	}

	explicit Bulge() = default;

	const QVector2D& start() const { return m_start; }

	QVector2D& start() { return m_start; }

	const QVector2D& end() const { return m_end; }

	QVector2D& end() { return m_end; }

	float tangent() const { return m_tangent; }

	float& tangent() { return m_tangent; }

	float length() const
	{
		if (isLine()) {
			return m_start.distanceToPoint(m_end);
		}

		// Radius is half line length * 1 + t^2 / (4 * |t|)
		const float radius = m_start.distanceToPoint(m_end) * (1.0f + m_tangent * m_tangent) / m_tangent;
		const float angle = std::atan(m_tangent);
		return radius * angle;
	}

	Rect boundingRect() const
	{
		const Rect lineBoundingRect(m_start, m_end);
		if (isArc()) {
			const Point2DList arcPoints = arcBoundingPoints();

			if (arcPoints.empty()) {
				return lineBoundingRect;
			}

			const Rect arcPointBoundingRect = boundingRectPoints(arcPoints);
			return lineBoundingRect | arcPointBoundingRect;
		}

		return lineBoundingRect;
	}

	/// Change direction
	void invert()
	{
		std::swap(m_start, m_end);
		m_tangent = -m_tangent;
	}

	/// Transform to line, means tangent is 0.
	void linify() { m_tangent = 0.0f; }

	// Extend bulge start point
	Bulge extendStart(const QVector2D& start) const { return Bulge(start, m_end, m_tangent); }

	// Extend bulge end point
	Bulge extendEnd(const QVector2D& end) const { return Bulge(m_start, end, m_tangent); }

	bool isLine() const
	{
		return std::abs(m_tangent) < std::numeric_limits<float>::epsilon(); // TODO utils
	}

	bool isArc() const { return !isLine(); }

	Orientation orientation() const { return (m_tangent < 0.0f) ? Orientation::CW : Orientation::CCW; }

	Circle toCircle() const
	{
		const Orientation ori = orientation();
		const float radius = arcRadius();
		const QVector2D center = m_start + relativeArcCenter();

		return Circle(center, radius, ori);
	}

	Arc toArc() const
	{
		const Circle circle = toCircle();
		const QVector2D& center = circle.center();

		const float startAngle = LineAngle(m_start - center);
		const float endAngle = LineAngle(m_end - center);

		return Arc(circle, m_start, m_end, startAngle, endAngle);
	}

	Line toLine() const { return Line(m_start, m_end); }

	void transform(const QTransform& matrix)
	{
		m_start = mapVector2D(m_start, matrix);
		m_end = mapVector2D(m_end, matrix);

		if (matrix.isScaling()) {
			const bool invertTagent = (matrix.m11() * matrix.m22()) < 0.0f;
			if (invertTagent) {
				m_tangent = -m_tangent;
			}
		}
	}

	bool operator==(const Bulge& other) const
	{
		return m_start == other.m_start && m_end == other.m_end && m_tangent == other.m_tangent;
	}

	bool equalsInversed(const Bulge& other) const
	{
		return m_start == other.m_end && m_end == other.m_start && m_tangent == -other.m_tangent;
	}
};

}
