#include <quadraticspline.h>

namespace Geometry
{

Point2DList QuadraticSpline::convertClosedToQuadraticBezierPoints() const
{
	const Point2DList &allControlPoints = controlPoints();
	const int nbcontrol = allControlPoints.size();

	const int size = nbcontrol * 2 + 1;
	Point2DList bezierPoints(size);

	// Copy 1 ... -2 -1 to 1 3 .. -4 -2
	for (int i = 1, dst = 1; i <= nbcontrol; ++i, dst += 2) {
		const int src = i % nbcontrol;
		bezierPoints[dst] = allControlPoints[src];
	}

	for (int i = 0; i <= nbcontrol; ++i) {
		const int src = i % nbcontrol;
		const int dst = i * 2;
		const int srcnext = (src + 1) % nbcontrol;
		bezierPoints[dst] = (allControlPoints[src] + allControlPoints[srcnext]) / 2.0f;
	}

	return bezierPoints;
}

Point2DList QuadraticSpline::convertOpenedToQuadraticBezierPoints() const
{
	const Point2DList &allControlPoints = controlPoints();
	const int nbcontrol = allControlPoints.size();

	const int size = (nbcontrol - 1) * 2 - 1;
	Point2DList bezierPoints(size);

	for (int src = 1, dst = 1; src < (nbcontrol - 1); ++src, dst += 2) {
		bezierPoints[dst] = allControlPoints[src];
	}

	for (int src = 1; src < (nbcontrol - 2); ++src) {
		const int dst = src * 2;
		const int srcnext = src + 1;
		bezierPoints[dst] = (allControlPoints[src] + allControlPoints[srcnext]) / 2.0f;
	}

	bezierPoints.front() = allControlPoints.front();
	bezierPoints.back() = allControlPoints.back();

	return bezierPoints;
}

static Bezier quadraticBezierPointToCubicBezier(const QVector2D &q0, const QVector2D &q1, const QVector2D &q2)
{
	const QVector2D c1 = q0 + 2.0f * (q1 - q0) / 3.0f;
	const QVector2D c2 = q2 + 2.0f * (q1 - q2) / 3.0f;

	return Bezier(q0, c1, c2, q2);
}

Bezier::List QuadraticSpline::pointsToBeziers(const Point2DList &bezierPoints) const
{
	const int size = bezierPoints.size();
	Bezier::List beziers(size / 2);

	for (int src = 0, dst = 0; src < (size - 1); src += 2, ++dst) {
		beziers[dst] = quadraticBezierPointToCubicBezier(bezierPoints[src], bezierPoints[src + 1], bezierPoints[src + 2]);
	}

	return beziers;
}

QuadraticSpline::QuadraticSpline(Point2DList &&points, bool closed)
	:Spline(std::move(points), closed)
{
}

Bezier::List QuadraticSpline::toBeziers() const
{
	const Point2DList bezierPoints = closed() ? convertClosedToQuadraticBezierPoints() : convertOpenedToQuadraticBezierPoints();

	return pointsToBeziers(bezierPoints);
}

}
