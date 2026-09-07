module;

#include <QVector2D>

export module geometry.circle;

import geometry.utils;

export namespace geometry
{

class Circle
{
private:
	QVector2D m_center;
	float m_radius;
	Orientation m_orientation;

public:
	explicit Circle(const QVector2D& center, float radius, Orientation orientation);

	const QVector2D& center() const;
	float radius() const;
	Orientation orientation() const;
};

}

namespace geometry
{

Circle::Circle(const QVector2D& center, float radius, Orientation orientation)
	: m_center(center)
	, m_radius(radius)
	, m_orientation(orientation)
{
}

const QVector2D& Circle::center() const
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
