#include <spline.h>

namespace geometry
{

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
