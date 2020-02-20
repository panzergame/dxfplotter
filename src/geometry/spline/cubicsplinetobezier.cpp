#include <cubicsplinetobezier.h>

#include <libdxfrw/libdxfrw.h>

#include <QDebug>

namespace Geometry::Spline
{

CubicSplineToBezier::CubicSplineToBezier(Point2DList &&controlPoints, bool closed)
	:SplineToBezier(std::move(controlPoints), closed)
{
	if (m_closed) {
		convertClosed();
	}
	else {
		convertOpened();
	}

	qInfo() << "bezier points :";
	for (const QVector2D &point : m_bezierPoints) {
		qInfo() << point;
	}
}

void CubicSplineToBezier::convertClosed()
{
	const int nbcontrol = m_controlPoints.size();
	const int lastcontrol = nbcontrol - 1;

	const int size = nbcontrol * 3 + 1;

	m_bezierPoints.resize(size);
	for (QVector2D& p : m_bezierPoints) { // TODO
		p = QVector2D(-1, -1);
	}

	// Copy of 0 ... -1 0 to 0 3 6 ... -10 -7 -4 0
	for (int i = 0; i <= nbcontrol; ++i) {
		const int src = i % nbcontrol;
		const int dst = i * 3;
		m_bezierPoints[dst] = m_controlPoints[src];
	}

	for (int src = 0, dst = 1; src < nbcontrol; ++src, dst += 3) {
		const int srcnext = (src + 1) % nbcontrol;
		m_bezierPoints[dst] = (m_controlPoints[src] * 2.0f + m_controlPoints[srcnext]) / 3.0f;
		m_bezierPoints[dst + 1] = (m_controlPoints[src] + m_controlPoints[srcnext] * 2.0f) / 3.0f;
	}

	for (int i = 3; i < size - 3; i += 3) {
		m_bezierPoints[i] = ((m_bezierPoints[i - 1] + m_bezierPoints[i + 1]) / 2.0f);
	}

	m_bezierPoints[0] = (m_bezierPoints[size - 2] + m_bezierPoints[1]) / 2.0f;
	m_bezierPoints[size - 1] = m_bezierPoints[0];
}

void CubicSplineToBezier::convertOpened()
{
	/*const int size = m_controlPoints.size();

	// Simply a bezier
	if (size == 4) {
		m_bezierPoints = m_controlPoints;
	}
	else {
		for (int i = 0; i < (size - 1); ++i) {
			const bool firstOrLast = (i == 0) || (i == (size - 2));

			addPoint(i);

			if (!firstOrLast) {
				const bool secondOrLast = (i == 1) || (i == (size - 3));

				if (secondOrLast) {
					addInterpolatedPoint(i, 0.5f, i + 1, 0.5f);
				}
				else {
					addInterpolatedPoint(i, 2.0f / 3.0f, i + 1, 1.0f / 3.0f);
					addInterpolatedPoint(i, 1.0f / 3.0f, i + 1, 2.0f / 3.0f);
				}
			}
		}

		addPoint(size - 1);
	}

	for (int i = 3; i < (size - 1); i += 3) {
		m_bezierPoints[i] = ((m_bezierPoints[i - 1] + m_bezierPoints[i + 1]) / 2.0);
	}*/

	const int nbcontrol = m_controlPoints.size();
	const int lastcontrol = nbcontrol - 1;

	int size = nbcontrol;
	if (nbcontrol > 4) {
		const int nbpairs = nbcontrol - 1;

		size += 2; // Point deuxième et penultième
		size += (nbpairs - 4) * 2;
	}

	m_bezierPoints.resize(size);
	for (QVector2D& p : m_bezierPoints) { // TODO
		p = QVector2D(-1, -1);
	}

	const int last = size - 1;

	// Copy of two begin and end raw points 0 1 -2 -1 to 0 1 -2 -1
	for (int src = 0; src < 2; ++src) {
		// Copy from begin
		m_bezierPoints[src] = m_controlPoints[src];
		// Copy from end
		m_bezierPoints[last - src] = m_controlPoints[lastcontrol - src];
	}

	// Copy of remaining raw points from 2 ... -3 to 3 6 9 ... -10 -7 -4 (every 3 with 6 point distance from begin and end).
	for (int src = 2, dst = 3; src < (nbcontrol - 2); ++src, dst += 3) {
		m_bezierPoints[dst] = m_controlPoints[src];
	}

	if (nbcontrol > 4) {
		// Copy half points 2 and -3
		m_bezierPoints[2] = (m_controlPoints[1] + m_controlPoints[2]) / 2.0f;
		m_bezierPoints[last - 2] = (m_controlPoints[lastcontrol - 1] + m_controlPoints[lastcontrol - 2]) / 2.0f;
	}

	// Copy third point 2 ... -3 to (4 5) (7 8) .. (-9 -8) (-6 -5)
	for (int src = 2, dst = 4; src < (nbcontrol - 3); ++src, dst += 3) {
		m_bezierPoints[dst] = (m_controlPoints[src] * 2.0f + m_controlPoints[src + 1]) / 3.0f;
		m_bezierPoints[dst + 1] = (m_controlPoints[src] + m_controlPoints[src + 1] * 2.0f) / 3.0f;
	}

	for (int i = 3; i < (size - 1); i += 3) {
		m_bezierPoints[i] = ((m_bezierPoints[i - 1] + m_bezierPoints[i + 1]) / 2.0f);
	}
}

}
