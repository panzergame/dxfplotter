#pragma once

#include <geometry/utils.h>

namespace geometry
{

class Circle
{
private:
	QVector2D m_center;
	float m_radius;
	Orientation m_orientation;

public:
	explicit Circle(const QVector2D &center, float radius, Orientation orientation);

	const QVector2D &center() const;
	float radius() const;
	Orientation orientation() const;
};

}
