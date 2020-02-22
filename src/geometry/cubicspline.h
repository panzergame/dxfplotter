#pragma once

#include <geometry/spline.h>

namespace Geometry
{

class CubicSpline : public Spline
{
private:
	Point2DList convertClosedToBezierPoints() const;
	Point2DList convertOpenedToBezierPoints() const;

public:
	explicit CubicSpline(Point2DList &&points, bool closed);

	Bezier::List beziers() const;
};

}
