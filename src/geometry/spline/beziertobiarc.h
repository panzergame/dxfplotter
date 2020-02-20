#pragma once

#include <geometry/spline/bezier.h>

namespace Geometry::Spline
{

class BezierToBiarc
{
private:
	Bezier::List m_beziers;
	Bezier::List m_convexBezier;

public:
	explicit BezierToBiarc(Bezier::List &&beziers);
};

}
