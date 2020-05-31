#pragma once

#include <geometry/utils.h>

namespace Geometry
{

class Circle
{
protected:
	QVector2D m_center;
	float m_radius;
	Orientation m_orientation;

public:
	explicit Circle(const QVector2D &center, float radius, Orientation orientation);

	const QVector2D &center() const;
	float radius() const;
	Orientation orientation() const;
};

};
