#pragma once

#include <geometry/utils.h>
#include <geometry/spline/bezier.h>

#include <vector>

namespace Geometry::Spline
{

class SplineToBezier
{
protected:
	Point2DList m_controlPoints;
	Point2DList m_bezierPoints;

	bool m_closed;

public:
	explicit SplineToBezier(Point2DList &&controlPoints, bool closed);

	Bezier::List beziers() const;
};

}
