#include <bulge.h>
#include <utils.h>
#include <limits>

#include <QDebug> // TODO
#include <iostream> // TODO
#include <iomanip>
#include <limits>

namespace geometry
{

Bulge::Bulge(const Eigen::Vector2d &start, const Eigen::Vector2d &end, double tangent)
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

const Eigen::Vector2d &Bulge::start() const
{
	return m_start;
}

Eigen::Vector2d &Bulge::start()
{
	return m_start;
}

const Eigen::Vector2d &Bulge::end() const
{
	return m_end;
}

Eigen::Vector2d &Bulge::end()
{
	return m_end;
}

double Bulge::tangent() const
{
	return m_tangent;
}

double &Bulge::tangent()
{
	return m_tangent;
}

double Bulge::length() const
{
	const double lineLength = (m_end - m_start).norm();
	if (isLine()) {
		return lineLength;
	}

	// Radius is half line length * 1 + t^2 / (4 * |t|)
	const double radius = lineLength * (1.0 + m_tangent * m_tangent) / m_tangent;
	const double angle = std::atan(m_tangent);
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

Bulge Bulge::extendStart(const Eigen::Vector2d &start) const
{
	return Bulge(start, m_end, m_tangent);
}

Bulge Bulge::extendEnd(const Eigen::Vector2d &end) const
{
	return Bulge(m_start, end, m_tangent);
}

bool Bulge::isLine() const
{
	return std::abs(m_tangent) < std::numeric_limits<double>::epsilon(); // TODO utils
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

	const double absTangent = std::abs(m_tangent);

	// Tangent is at end point, so we get line from end to start.
	const Eigen::Vector2d line = m_start - m_end;

	const double lineLength = line.norm();
	const double radius = (lineLength * (1.0 + m_tangent * m_tangent)) / (4.0 * absTangent);

	// Angle of line end -> start
	const double lineAngle = LineAngle(line);
	// Angle between line and line from end to middle of arc
	const double absTheta4 = std::atan(absTangent);

	// Absolute angle at end point from line to arc center.
	const double relativeAngleToCenter = M_PI_2 - 2.0 * absTheta4;
	const double angleToCenter = (ori == Orientation::CCW) ? (lineAngle - relativeAngleToCenter) : (lineAngle + relativeAngleToCenter);

	const Eigen::Vector2d relativeCenter(std::cos(angleToCenter) * radius, std::sin(angleToCenter) * radius);
	const Eigen::Vector2d center = relativeCenter + m_end;

	return Circle(center, radius, ori);
}

Arc Bulge::toArc() const
{
	const Circle circle = toCircle();
	const Eigen::Vector2d &center = circle.center();

	const double startAngle = LineAngle(m_start - center);
	const double endAngle = LineAngle(m_end - center);

	return Arc(circle, m_start, m_end, startAngle, endAngle);
}

inline Eigen::Vector2d mapVector2D(const Eigen::Vector2d &vect, const Eigen::Affine2d &matrix)
{
	return matrix * vect;
}

void Bulge::transform(const Eigen::Affine2d &matrix)
{
	m_start = mapVector2D(m_start, matrix);
	m_end = mapVector2D(m_end, matrix);

	// TODO
	/*if (matrix.isScaling()) {
		const bool invertTagent = (matrix.m11() * matrix.m22()) < 0.0f;
		if (invertTagent) {
			m_tangent = -m_tangent;
		}
	}*/
}

bool Bulge::operator==(const Bulge& other) const
{
	return m_start == other.m_start && m_end == other.m_end && m_tangent == other.m_tangent;
}

}
