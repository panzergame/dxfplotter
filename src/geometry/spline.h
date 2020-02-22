#pragma once

#include <geometry/bezier.h>
#include <geometry/utils.h>

namespace Geometry
{

class Spline
{
protected:
	Point2DList m_points;
	bool m_closed;

	Bezier::List toBeziers(const Point2DList &bezierPoints) const;

public:
	explicit Spline(Point2DList &&points, bool closed);
};

}
