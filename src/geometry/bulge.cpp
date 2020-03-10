#include <bulge.h>
#include <utils.h>
#include <limits>

#include <QDebug> // TODO
#include <iostream> // TODO
#include <iomanip>
#include <limits>

namespace Geometry
{

Bulge::Bulge(const QVector2D &start, const QVector2D &end, float tangent)
	:m_start(start),
	m_end(end),
	m_tangent(tangent)
{
}

const QVector2D &Bulge::start() const
{
	return m_start;
}

const QVector2D &Bulge::end() const
{
	return m_end;
}

void Bulge::invert()
{
	std::swap(m_start, m_end);
	m_tangent = -m_tangent;
}

bool Bulge::isLine() const
{
	return std::abs(m_tangent) < std::numeric_limits<float>::epsilon(); // TODO utils
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

	const float midline = line.length() / 2.0f;
	const float sagitta = midline * absTangent;
	const float radius = (sagitta * sagitta + midline * midline) / (2.0f * sagitta);

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
	const float radius = circle.radius();

	const float startAngle = LineAngle(m_start - center);
	const float endAngle = LineAngle(m_end - center);

	return Arc(circle, m_start, m_end, startAngle, endAngle);
}

}
