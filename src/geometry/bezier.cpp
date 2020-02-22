#include <bezier.h>
#include <geometry/utils.h>

#include <QDebug> // TODO
#include <iostream> // TODO

namespace Geometry
{

Bezier::Bezier(const QVector2D &p1, const QVector2D &c1, const QVector2D &c2, const QVector2D &p2)
	:m_point1(p1),
	m_point2(p2),
	m_control1(c1),
	m_control2(c2)
{
}

const QVector2D &Bezier::point1() const
{
	return m_point1;
}

const QVector2D &Bezier::point2() const
{
	return m_point2;
}

const QVector2D &Bezier::control1() const
{
	return m_control1;
}

const QVector2D &Bezier::control2() const
{
	return m_control2;
}

Bezier::InflexionPoints Bezier::inflexions() const
{
	const QVector2D A = m_control1 - m_point1;
	const QVector2D B = m_control2 - m_control1 - A;
	const QVector2D C = m_point2 - m_control2 - A - 2.0f * B;
    
	const Complex a(B.x() * C.y() - B.y() * C.x(), 0.0f);
	const Complex b(A.x() * C.y() - A.y() * C.x(), 0.0f);
	const Complex c(A.x() * B.y() - A.y() * B.x(), 0.0f);
    
	const Complex d = sqrt(b * b - 4.0f * a * c);
	const Complex e = 2.0f * a;

	Complex t1;
	Complex t2;

	/* If control points are opposed and parrallel, e is zero and
	 * inflexion point is at half the curve.
	 */
	if (std::norm(e) == 0.0f) {	// TODO epsilon
		t1 = Complex(0.5f, 0.0f);
		t2 = Complex(0.0f, 0.0f);
	}
	else {
		t1 = (-b + d) / e;
		t2 = (-b - d) / e;
	}

	return {t1, t2};
}

Bezier::Pair Bezier::split(float t) const
{
	const QVector2D p0 = m_point1 + t * (m_control1 - m_point1);
	const QVector2D p1 = m_control1 + t * (m_control2 - m_control1);
	const QVector2D p2 = m_control2 + t * (m_point2 - m_control2);

	const QVector2D p01 = p0 + t * (p1 - p0);
	const QVector2D p12 = p1 + t * (p2 - p1);

	const QVector2D dp = p01 + t * (p12 - p01);
    
	const Bezier b1(m_point1, p0, p01, dp);
	const Bezier b2(dp, p12, p2, m_point2);

	return {b1, b2};
}

Bezier::Pair Bezier::splitHalf() const
{
	const QVector2D qc = (m_control1 + m_control2) / 4.0f;

	const QVector2D r2 = (m_point1 + m_control1) / 2.0f;
	const QVector2D r3 = r2 / 2.0f + qc;

	const QVector2D s3 = (m_control2 + m_point2) / 2.0f;
	const QVector2D s2 = s3 / 2.0f + qc;
	const QVector2D dp = (r3 + s2) / 2.0f;

	const Bezier b1(m_point1, r2, r3, dp);
	const Bezier b2(dp, s2, s3, m_point2);

	return {b1, b2};
}

QVector2D Bezier::at(float t) const
{
	const float ot = 1.0f - t;

	return (ot * ot * ot) * m_point1 +
			(3.0f * (ot * ot) * t) * m_control1 +
			(3.0f * ot * (t * t)) * m_control2 +
			(t * t * t) * m_point2;
}

bool Bezier::isRealInflexionPoint(const Bezier::Complex &point)
{
	return (point.real() > 0.0f && point.real() < 1.0f && point.imag() == 0.0f); // TODO epsilon
}

Bezier::List Bezier::splitToConvex() const
{
	const InflexionPoints inflex = inflexions();
	const bool isReal1 = isRealInflexionPoint(inflex[0]);
	const bool isReal2 = isRealInflexionPoint(inflex[1]);

	std::cout << inflex[0] << " " << inflex[1] << std::endl;
	qInfo() << isReal1 << isReal2;

	// Split at first point
	if (isReal1 && !isReal2) {
		const Pair splitted = split(inflex[0].real());

		return {splitted[0], splitted[1]};
	}
	// Split at second point
	if (!isReal1 && isReal2) {
		const Pair splitted = split(inflex[1].real());

		return {splitted[0], splitted[1]};
	}
	// Split at both point
	if (isReal1 && isReal2) {
		// TODO besoin d'ordonner ?
		const float t1 = inflex[0].real();
		const float t2 = (1.0f - t1) * inflex[1].real();

		const Pair splitted1 = split(t1);
		const Pair splitted2 = split(t2);

		return {splitted1[0], splitted2[0], splitted2[1]};
	}

	// No split
	return {*this};
}

QVector2D Bezier::incenter() const
{
	/*const QVector2D v = LineIntersection(m_point1, m_control1, m_point2, m_control2);

	qInfo() << "intersection :"  << v;

	return v;*/
	return QVector2D();
}

}
