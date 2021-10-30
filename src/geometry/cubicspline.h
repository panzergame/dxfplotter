#pragma once

#include <geometry/spline.h>

namespace geometry
{

class CubicSpline : public Spline
{
private:
	Point2DList convertClosedToCubicBezierPoints() const;
	Point2DList convertOpenedToCubicBezierPoints() const;

	Bezier::List pointsToBeziers(const Point2DList &bezierPoints) const;

public:
	explicit CubicSpline(Point2DList &&points, bool closed);

	Bezier::List toBeziers() const;
};

}
