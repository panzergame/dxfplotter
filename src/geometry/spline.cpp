module;

#include <utility>
#include <QVector2D>

export module geometry.spline;

import geometry.bezier;
import geometry.utils;

export namespace geometry
{

class Spline
{
private:
	Point2DList m_controlPoints;
	bool m_closed;

protected:
	const Point2DList& controlPoints() const;
	bool closed() const;

public:
	explicit Spline(Point2DList&& points, bool closed);
};

}

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

Spline::Spline(Point2DList&& points, bool closed)
	: m_controlPoints(points)
	, m_closed(closed)
{
}

}
