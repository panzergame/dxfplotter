#include <circle.h>

namespace Geometry
{

Circle::Circle(const QVector2D &center, float radius, Orientation orientation)
	:m_center(center),
	m_radius(radius),
	m_orientation(orientation)
{
}

const QVector2D &Circle::center() const
{
	return m_center;
}

float Circle::radius() const
{
	return m_radius;
}

Orientation Circle::orientation() const
{
	return m_orientation;
}

}
