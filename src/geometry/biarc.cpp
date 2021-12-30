#include <biarc.h>
#include <utils.h>
#include <bulge.h>

#include <QDebug>
#include <QtMath>

namespace geometry
{

Orientation Biarc::orientation() const
{
	const double det = (m_middle.x() - m_point1.x()) * (m_middle.y() + m_point1.y()) +
			(m_point2.x() - m_middle.x()) * (m_point2.y() + m_middle.y()) +
			(m_point1.x() - m_point2.x()) * (m_point1.y() + m_point2.y());

	return (det < 0.0f) ? Orientation::CW : Orientation::CCW;
}

Biarc::Biarc(const Eigen::Vector2d& point1, const Eigen::Vector2d& middle, const Eigen::Vector2d& point2,
		const Eigen::Vector2d& tangent1, const Eigen::Vector2d& tangent2)
	:m_point1(point1),
	m_point2(point2),
	m_middle(middle),
	m_tangent1(tangent1),
	m_tangent2(tangent2),
	m_line1(middle - point1),
	m_line2(middle - point2)
{
}

const Eigen::Vector2d &Biarc::middle() const
{
	return m_middle;
}

Eigen::Vector2d Biarc::tangentAtMiddle() const
{
	// Rotate line by PI/2
	const Eigen::Vector2d normalizedLine1 = m_line1.normalized();
	const Eigen::Vector2d perpendicularLine1 = PerpendicularLine(normalizedLine1);

	// Tangent at middle is the reflect of tangent at start by perpendicular line start to end.
	return ReflectLine(m_tangent1.normalized(), perpendicularLine1);
}

Polyline Biarc::toPolyline() const
{
	/* Angle from end to start line with arc tangent at start point is double of
	 * bulge tangent angle.
	 */

	// Half angle from tangent1 to line1
	const double thetab1 = (LineAngle(m_line1) - LineAngle(m_tangent1)) / 2.0f;
	// Half angle from line2 to tangent2
	const double thetab2 = (LineAngle(m_tangent2) - LineAngle(m_line2)) / 2.0f;

	const Bulge b1(m_point1, m_middle, std::tan(thetab1));
	const Bulge b2(m_middle, m_point2, std::tan(thetab2));

	return Polyline({b1, b2});
}

}

