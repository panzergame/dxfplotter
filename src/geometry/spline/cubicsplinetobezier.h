#pragma once

#include <geometry/spline/splinetobezier.h>

namespace Geometry::Spline
{

class CubicSplineToBezier : public SplineToBezier
{
	void convertClosed();
	void convertOpened();

public:
	explicit CubicSplineToBezier(Point2DList &&controlPoints, bool closed);
};

}
