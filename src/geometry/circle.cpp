#include <circle.h>

namespace geometry
{

Circle::Circle(const Eigen::Vector2d &center, double radius, Orientation orientation)
	:m_center(center),
	m_radius(radius),
	m_orientation(orientation)
{
}

const Eigen::Vector2d &Circle::center() const
{
	return m_center;
}

double Circle::radius() const
{
	return m_radius;
}

Orientation Circle::orientation() const
{
	return m_orientation;
}

}
