#include <cubicspline.h>

#include <QDebug>

namespace geometry
{

CubicSpline::CubicSpline(Point2DList &&points, bool closed)
	:Spline(std::move(points), closed)
{
}

Point2DList CubicSpline::convertClosedToCubicBezierPoints() const
{
	const Point2DList &allControlPoints = controlPoints();
	const int nbcontrol = allControlPoints.size();

	const int size = nbcontrol * 3 + 1;

	Point2DList bezierPoints(size);

	// Copy of 0 ... -1 0 to 0 3 6 ... -10 -7 -4 0
	for (int i = 0; i <= nbcontrol; ++i) {
		const int src = i % nbcontrol;
		const int dst = i * 3;
		bezierPoints[dst] = allControlPoints[src];
	}

	for (int src = 0, dst = 1; src < nbcontrol; ++src, dst += 3) {
		const int srcnext = (src + 1) % nbcontrol;
		bezierPoints[dst] = (allControlPoints[src] * 2.0f + allControlPoints[srcnext]) / 3.0f;
		bezierPoints[dst + 1] = (allControlPoints[src] + allControlPoints[srcnext] * 2.0f) / 3.0f;
	}

	for (int i = 3; i < size - 3; i += 3) {
		bezierPoints[i] = ((bezierPoints[i - 1] + bezierPoints[i + 1]) / 2.0f);
	}

	bezierPoints.front() = (bezierPoints[size - 2] + bezierPoints[1]) / 2.0f;
	bezierPoints.back() = bezierPoints.front();

	return bezierPoints;
}

Bezier::List CubicSpline::pointsToBeziers(const Point2DList &bezierPoints) const
{
	const int size = bezierPoints.size();
	Bezier::List beziers(size / 3);

	for (int src = 0, dst = 0; src < (size - 1); src += 3, ++dst) {
		beziers[dst] = Bezier(bezierPoints[src], bezierPoints[src + 1], bezierPoints[src + 2], bezierPoints[src + 3]);
	}

	return beziers;
}

Point2DList CubicSpline::convertOpenedToCubicBezierPoints() const
{
	const Point2DList &allControlPoints = controlPoints();
	const int nbcontrol = allControlPoints.size();
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
		bezierPoints[src] = allControlPoints[src];
		// Copy from end
		bezierPoints[last - src] = allControlPoints[lastcontrol - src];
	}

	// Copy of remaining raw points from 2 ... -3 to 3 6 9 ... -10 -7 -4 (every 3 with 6 point distance from begin and end).
	for (int src = 2, dst = 3; src < (nbcontrol - 2); ++src, dst += 3) {
		bezierPoints[dst] = allControlPoints[src];
	}

	if (nbcontrol > 4) {
		// Copy half points 2 and -3
		bezierPoints[2] = (allControlPoints[1] + allControlPoints[2]) / 2.0f;
		bezierPoints[last - 2] = (allControlPoints[lastcontrol - 1] + allControlPoints[lastcontrol - 2]) / 2.0f;
	}

	// Copy third point 2 ... -3 to (4 5) (7 8) .. (-9 -8) (-6 -5)
	for (int src = 2, dst = 4; src < (nbcontrol - 3); ++src, dst += 3) {
		bezierPoints[dst] = (allControlPoints[src] * 2.0f + allControlPoints[src + 1]) / 3.0f;
		bezierPoints[dst + 1] = (allControlPoints[src] + allControlPoints[src + 1] * 2.0f) / 3.0f;
	}

	for (int i = 3; i < (size - 1); i += 3) {
		bezierPoints[i] = ((bezierPoints[i - 1] + bezierPoints[i + 1]) / 2.0f);
	}

	return bezierPoints;
}

Bezier::List CubicSpline::toBeziers() const
{
	const Point2DList bezierPoints = closed() ? convertClosedToCubicBezierPoints() : convertOpenedToCubicBezierPoints();

	return pointsToBeziers(bezierPoints);
}


}
