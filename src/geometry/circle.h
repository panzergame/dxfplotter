#pragma once

#include <geometry/utils.h>

namespace geometry
{

class Circle
{
private:
	Eigen::Vector2d m_center;
	double m_radius;
	Orientation m_orientation;

public:
	explicit Circle(const Eigen::Vector2d &center, double radius, Orientation orientation);

	const Eigen::Vector2d &center() const;
	double radius() const;
	Orientation orientation() const;
};

};
