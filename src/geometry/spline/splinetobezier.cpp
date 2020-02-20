#include <splinetobezier.h>

namespace Geometry::Spline
{

SplineToBezier::SplineToBezier(Point2DList &&controlPoints, bool closed)
	:m_controlPoints(controlPoints),
	m_closed(closed)
{
}

Bezier::List SplineToBezier::beziers() const
{
	const int size = m_bezierPoints.size();
	Bezier::List beziers(size / 3);

	for (int src = 0, dst = 0; src < (size - 1); src += 3, ++dst) {
		beziers[dst] = Bezier(m_bezierPoints[src], m_bezierPoints[src + 1], m_bezierPoints[src + 2], m_bezierPoints[src + 3]);
	}

	return beziers;
}

}
