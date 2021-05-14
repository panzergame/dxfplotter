#include <spline.h>

namespace Geometry
{

Bezier::List Spline::pointsToBeziers(const Point2DList &bezierPoints) const
{
	const int size = bezierPoints.size();
	Bezier::List beziers(size / 3);

	for (int src = 0, dst = 0; src < (size - 1); src += 3, ++dst) {
		beziers[dst] = Bezier(bezierPoints[src], bezierPoints[src + 1], bezierPoints[src + 2], bezierPoints[src + 3]);
	}

	return beziers;
}

const Point2DList& Spline::controlPoints() const
{
	return m_controlPoints;
}

bool Spline::closed() const
{
	return m_closed;
}

Spline::Spline(Point2DList &&points, bool closed)
	:m_controlPoints(points),
	m_closed(closed)
{
}

}
