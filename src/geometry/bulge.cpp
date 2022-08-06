#include <bulge.h>
#include <cmath>
#include <iterator>
#include <utils.h>
#include <limits>

#include <QDebug> // TODO
#include <iostream> // TODO
#include <iomanip>
#include <limits>

namespace geometry
{

float Bulge::arcRadius() const
{
	const float absTangent = std::abs(m_tangent);

	const QVector2D line = m_end - m_start;
	const float lineLength = line.length();
	const float radius = (lineLength * (1.0f + m_tangent * m_tangent)) / (4.0f * absTangent);

	return radius;
}

QVector2D Bulge::relativeArcCenter() const
{
	const QVector2D line = m_end - m_start;
	const QVector2D halfLine = line / 2.0f;

	const float theta4 = std::atan(m_tangent);
	const float lineToCenterAngle = M_PI_2 - theta4 * 2.0f;

	if (lineToCenterAngle < precision::fuzzyAngle) {
		return halfLine;
	}
	else {
		const float lineToCenterTangent = std::tan(lineToCenterAngle);

		const QVector2D perpendicularHalfLine(-halfLine.y(), halfLine.x());
		const QVector2D relativeCenter = halfLine + perpendicularHalfLine * lineToCenterTangent;
		return relativeCenter;
	}
}

Point2DList Bulge::arcBoundingPoints() const
{
	Point2DList points;

	const QVector2D relativeCenter = relativeArcCenter();
	const QVector2D line = m_end - m_start;
	const float radius = arcRadius();

	static const QVector2D quadrantPointsUnit[4] = {
		QVector2D(1, 0),
		QVector2D(0, 1),
		QVector2D(-1, 0),
		QVector2D(0, -1)
	};

	const bool centerLineSide = std::signbit(CrossProduct(line, relativeCenter));

	QVector2D quadrantPoints[4];
	std::transform(quadrantPointsUnit, quadrantPointsUnit + 4, quadrantPoints, [&relativeCenter, radius](const QVector2D& pointUnit){
		return relativeCenter + pointUnit * radius;
	});

	std::copy_if(quadrantPoints, quadrantPoints + 4, std::back_inserter(points),
		[centerLineSide, &line](const QVector2D &point){
			const bool pointLineSide = std::signbit(CrossProduct(line, point));
			const bool oppositeSideToCenter = (pointLineSide != centerLineSide);

			return oppositeSideToCenter;
		});

	std::transform(points.begin(), points.end(), points.begin(), [this](const QVector2D& point){
		return point + m_start;
	});

	return points;
}

Bulge::Bulge(const QVector2D &start, const QVector2D &end, float tangent)
	:m_start(start),
	m_end(end),
	m_tangent(tangent)
{
	assert(-1.0f <= tangent && tangent <= 1.0f);
}

Bulge::Bulge(const cavc::PlineVertex<double> &v1, const cavc::PlineVertex<double> &v2)
	:m_start(v1.x(), v1.y()),
	m_end(v2.x(), v2.y()),
	m_tangent(v1.bulge())
{
}

const QVector2D &Bulge::start() const
{
	return m_start;
}

QVector2D &Bulge::start()
{
	return m_start;
}

const QVector2D &Bulge::end() const
{
	return m_end;
}

QVector2D &Bulge::end()
{
	return m_end;
}

float Bulge::tangent() const
{
	return m_tangent;
}

float &Bulge::tangent()
{
	return m_tangent;
}

float Bulge::length() const
{
	if (isLine()) {
		return m_start.distanceToPoint(m_end);
	}

	// Radius is half line length * 1 + t^2 / (4 * |t|)
	const float radius = m_start.distanceToPoint(m_end) * (1.0f + m_tangent * m_tangent) / m_tangent;
	const float angle = std::atan(m_tangent);
	return radius * angle;
}

const Rect boundingRectPoints(const Point2DList &points)
{
	Point2DList::const_iterator it = points.begin();
	const QVector2D firstPoint = *(it++);

	return std::accumulate(it, points.end(), Rect(firstPoint));
}

Rect Bulge::boundingRect() const
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

void Bulge::invert()
{
	std::swap(m_start, m_end);
	m_tangent = -m_tangent;
}

void Bulge::linify()
{
	m_tangent = 0.0f;
}

Bulge Bulge::extendStart(const QVector2D &start) const
{
	return Bulge(start, m_end, m_tangent);
}

Bulge Bulge::extendEnd(const QVector2D &end) const
{
	return Bulge(m_start, end, m_tangent);
}

bool Bulge::isLine() const
{
	return std::abs(m_tangent) < std::numeric_limits<float>::epsilon(); // TODO utils
}

bool Bulge::isArc() const
{
	return !isLine();
}

Orientation Bulge::orientation() const
{
	return (m_tangent < 0.0f) ? Orientation::CW : Orientation::CCW;
}

Circle Bulge::toCircle() const
{
	const Orientation ori = orientation();
	const float radius = arcRadius();
	const QVector2D center = m_start + relativeArcCenter();

	return Circle(center, radius, ori);
}

Arc Bulge::toArc() const
{
	const Circle circle = toCircle();
	const QVector2D &center = circle.center();

	const float startAngle = LineAngle(m_start - center);
	const float endAngle = LineAngle(m_end - center);

	return Arc(circle, m_start, m_end, startAngle, endAngle);
}

inline QVector2D mapVector2D(const QVector2D &vect, const QTransform &matrix)
{
	const QPointF point = vect.toPointF();
	return QVector2D(matrix.map(point));
}

void Bulge::transform(const QTransform &matrix)
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

bool Bulge::operator==(const Bulge& other) const
{
	return m_start == other.m_start && m_end == other.m_end && m_tangent == other.m_tangent;
}

}
