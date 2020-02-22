#include <cubicspline.h>

#include <QDebug>

namespace Geometry
{

CubicSpline::CubicSpline(Point2DList &&points, bool closed)
	:Spline(std::move(points), closed)
{
}

Point2DList CubicSpline::convertClosedToBezierPoints() const
{
	const int nbcontrol = m_points.size();

	const int size = nbcontrol * 3 + 1;

	Point2DList bezierPoints(size);

	// Copy of 0 ... -1 0 to 0 3 6 ... -10 -7 -4 0
	for (int i = 0; i <= nbcontrol; ++i) {
		const int src = i % nbcontrol;
		const int dst = i * 3;
		bezierPoints[dst] = m_points[src];
	}

	for (int src = 0, dst = 1; src < nbcontrol; ++src, dst += 3) {
		const int srcnext = (src + 1) % nbcontrol;
		bezierPoints[dst] = (m_points[src] * 2.0f + m_points[srcnext]) / 3.0f;
		bezierPoints[dst + 1] = (m_points[src] + m_points[srcnext] * 2.0f) / 3.0f;
	}

	for (int i = 3; i < size - 3; i += 3) {
		bezierPoints[i] = ((bezierPoints[i - 1] + bezierPoints[i + 1]) / 2.0f);
	}

	bezierPoints[0] = (bezierPoints[size - 2] + bezierPoints[1]) / 2.0f;
	bezierPoints[size - 1] = bezierPoints[0];

	return bezierPoints;
}

Point2DList CubicSpline::convertOpenedToBezierPoints() const
{
	const int nbcontrol = m_points.size();
	const int lastcontrol = nbcontrol - 1;

	int size = nbcontrol;
	if (nbcontrol > 4) {
		const int nbpairs = nbcontrol - 1;

		size += 2; // Point deuxième et penultième
		size += (nbpairs - 4) * 2;
	}

	Point2DList bezierPoints(size);

	const int last = size - 1;

	// Copy of two begin and end raw points 0 1 -2 -1 to 0 1 -2 -1
	for (int src = 0; src < 2; ++src) {
		// Copy from begin
		bezierPoints[src] = m_points[src];
		// Copy from end
		bezierPoints[last - src] = m_points[lastcontrol - src];
	}

	// Copy of remaining raw points from 2 ... -3 to 3 6 9 ... -10 -7 -4 (every 3 with 6 point distance from begin and end).
	for (int src = 2, dst = 3; src < (nbcontrol - 2); ++src, dst += 3) {
		bezierPoints[dst] = m_points[src];
	}

	if (nbcontrol > 4) {
		// Copy half points 2 and -3
		bezierPoints[2] = (m_points[1] + m_points[2]) / 2.0f;
		bezierPoints[last - 2] = (m_points[lastcontrol - 1] + m_points[lastcontrol - 2]) / 2.0f;
	}

	// Copy third point 2 ... -3 to (4 5) (7 8) .. (-9 -8) (-6 -5)
	for (int src = 2, dst = 4; src < (nbcontrol - 3); ++src, dst += 3) {
		bezierPoints[dst] = (m_points[src] * 2.0f + m_points[src + 1]) / 3.0f;
		bezierPoints[dst + 1] = (m_points[src] + m_points[src + 1] * 2.0f) / 3.0f;
	}

	for (int i = 3; i < (size - 1); i += 3) {
		bezierPoints[i] = ((bezierPoints[i - 1] + bezierPoints[i + 1]) / 2.0f);
	}

	return bezierPoints;
}

Bezier::List CubicSpline::beziers() const
{
	const Point2DList bezierPoints = (m_closed) ? convertClosedToBezierPoints() : convertOpenedToBezierPoints();

	return toBeziers(bezierPoints);
}


}
