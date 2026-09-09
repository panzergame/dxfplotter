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
	explicit Circle(const QVector2D& center, float radius, Orientation orientation)
		: m_center(center)
		, m_radius(radius)
		, m_orientation(orientation)
	{
	}

	const QVector2D& center() const { return m_center; }

	float radius() const { return m_radius; }

	Orientation orientation() const { return m_orientation; }
};

}
