#pragma once

#include <geometry/bezier.h>
#include <geometry/utils.h>

namespace Geometry
{

class Spline
{
private:
	Point2DList m_controlPoints;
	bool m_closed;

protected:
	Bezier::List pointsToBeziers(const Point2DList &bezierPoints) const;

	const Point2DList& controlPoints() const;
	bool closed() const;

public:
	explicit Spline(Point2DList &&points, bool closed);
};

}
