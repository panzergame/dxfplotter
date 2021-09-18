#pragma once

#include <geometry/spline.h>

namespace Geometry
{

class QuadraticSpline : public Spline
{
private:
	Point2DList convertClosedToQuadraticBezierPoints() const;
	Point2DList convertOpenedToQuadraticBezierPoints() const;

	Bezier::List pointsToBeziers(const Point2DList &bezierPoints) const;

public:
	explicit QuadraticSpline(Point2DList &&points, bool closed);

	Bezier::List toBeziers() const;
};

}
