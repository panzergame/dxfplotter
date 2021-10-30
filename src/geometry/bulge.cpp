#include <bulge.h>
#include <utils.h>
#include <limits>

#include <QDebug> // TODO
#include <iostream> // TODO
#include <iomanip>
#include <limits>

namespace geometry
{

Bulge::Bulge(const QVector2D &start, const QVector2D &end, float tangent)
	:m_start(start),
	m_end(end),
	m_tangent(tangent)
{
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

	const float absTangent = std::abs(m_tangent);

	// Tangent is at end point, so we get line from end to start.
	const QVector2D line = m_start - m_end;

	const float lineLength = line.length();
	const float radius = (lineLength * (1.0f + m_tangent * m_tangent)) / (4.0f * absTangent);

	// Angle of line end -> start
	const float lineAngle = LineAngle(line);
	// Angle between line and line from end to middle of arc
	const float absTheta4 = std::atan(absTangent);

	// Absolute angle at end point from line to arc center.
	const float relativeAngleToCenter = M_PI_2 - 2.0f * absTheta4;
	const float angleToCenter = (ori == Orientation::CCW) ? (lineAngle - relativeAngleToCenter) : (lineAngle + relativeAngleToCenter);

	const QVector2D relativeCenter(std::cos(angleToCenter) * radius, std::sin(angleToCenter) * radius);
	const QVector2D center = relativeCenter + m_end;

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
}

bool Bulge::operator==(const Bulge& other) const
{
	return m_start == other.m_start && m_end == other.m_end && m_tangent == other.m_tangent;
}

}
