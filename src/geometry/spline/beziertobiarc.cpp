#include <beziertobiarc.h>

#include <QDebug> // TODO

namespace Geometry::Spline
{

BezierToBiarc::BezierToBiarc(Bezier::List &&beziers)
	:m_beziers(beziers)
{
	for (const Bezier& bezier : m_beziers) {
		const Bezier::List splitted = bezier.splitToConvex();
		m_convexBezier.insert(m_convexBezier.end(), splitted.begin(), splitted.end());
	}

	for (const Bezier& bezier : m_convexBezier) {
		/*qInfo() << bezier.point1();
		qInfo() << bezier.control1();
		qInfo() << bezier.control2();
		qInfo() << bezier.point2();*/

		fitArc(bezier);
	}
}

}
