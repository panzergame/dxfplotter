#include <bezier.h>
#include <geometry/utils.h>

namespace geometry
{

bool Bezier::isRealInflexionPoint(const Bezier::Complex &point)
{
	return (point.real() > 0.0f && point.real() < 1.0 && point.imag() == 0.0); // TODO epsilon
}

Bezier::InflexionPoints Bezier::inflexions() const
{
	const Eigen::Vector2d A = m_control1 - m_point1;
	const Eigen::Vector2d B = m_control2 - m_control1 - A;
	const Eigen::Vector2d C = m_point2 - m_control2 - A - 2.0f * B;
    
	const Complex a(B.x() * C.y() - B.y() * C.x(), 0.0f);
	const Complex b(A.x() * C.y() - A.y() * C.x(), 0.0f);
	const Complex c(A.x() * B.y() - A.y() * B.x(), 0.0f);
    
	const Complex d = sqrt(b * b - 4.0 * a * c);
	const Complex e = 2.0 * a;

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

Eigen::Vector2d Bezier::derivativeAt(double t) const
{
	const double s = 1.0 - t;
	const double s2 = s * s;
	const double t2 = t * t;

	return (-3.0f * m_point1 * s2 + 3.0f * m_control1 * (s2 - 2.0f * t * s) +
			3.0f * m_control2 * (2.0f * t * s - t2) + 3.0f * m_point2 * t2);
}

Eigen::Vector2d Bezier::findNearestPointWithTangent(const Eigen::Vector2d &point, const Eigen::Vector2d& tangent, double maxError) const
{
	double tn = 0.5f;
	Eigen::Vector2d Q_tn = at(tn);

	const double dpt = point.dot(tangent);
	double fn = Q_tn.dot(tangent) - dpt;

	while (std::abs(fn) > maxError) {
		const Eigen::Vector2d d_Q_tn = derivativeAt(tn);
		const double d_fn = d_Q_tn.dot(tangent);

		tn = tn - fn / d_fn;

		// Stop when out of range
		if (tn < 0.0f || tn > 1.0f) {
			break;
		}

		Q_tn = at(tn);
		fn = Q_tn.dot(tangent) - dpt;
	}

	return Q_tn;
}

Bezier::Bezier(const Eigen::Vector2d &p1, const Eigen::Vector2d &c1, const Eigen::Vector2d &c2, const Eigen::Vector2d &p2)
	:m_point1(p1),
	m_point2(p2),
	m_control1(c1),
	m_control2(c2)
{
}

const Eigen::Vector2d &Bezier::point1() const
{
	return m_point1;
}

const Eigen::Vector2d &Bezier::point2() const
{
	return m_point2;
}

const Eigen::Vector2d &Bezier::control1() const
{
	return m_control1;
}

const Eigen::Vector2d &Bezier::control2() const
{
	return m_control2;
}

Eigen::Vector2d Bezier::at(double t) const
{
	assert(0.0f <= t && t <= 1.0f);

	const double ot = 1.0f - t;

	return (ot * ot * ot) * m_point1 +
			(3.0f * (ot * ot) * t) * m_control1 +
			(3.0f * ot * (t * t)) * m_control2 +
			(t * t * t) * m_point2;
}

double Bezier::approximateLength() const
{
	const double chord = (m_point2 - m_point1).norm();
	const double controlNet = (m_control1 - m_point1).norm() +
		(m_control2 - m_control1).norm() +
		(m_point2 - m_control2).norm();

	return (chord + controlNet) / 2.0f;
}

Bezier::Pair Bezier::split(double t) const
{
	assert(0.0f < t && t < 1.0f);

	const Eigen::Vector2d p0 = m_point1 + t * (m_control1 - m_point1);
	const Eigen::Vector2d p1 = m_control1 + t * (m_control2 - m_control1);
	const Eigen::Vector2d p2 = m_control2 + t * (m_point2 - m_control2);

	const Eigen::Vector2d p01 = p0 + t * (p1 - p0);
	const Eigen::Vector2d p12 = p1 + t * (p2 - p1);

	const Eigen::Vector2d dp = p01 + t * (p12 - p01);

	const Bezier b1(m_point1, p0, p01, dp);
	const Bezier b2(dp, p12, p2, m_point2);

	return {b1, b2};
}

Bezier::Pair Bezier::splitHalf() const
{
	const Eigen::Vector2d qc = (m_control1 + m_control2) / 4.0f;

	const Eigen::Vector2d r2 = (m_point1 + m_control1) / 2.0f;
	const Eigen::Vector2d r3 = r2 / 2.0f + qc;

	const Eigen::Vector2d s3 = (m_control2 + m_point2) / 2.0f;
	const Eigen::Vector2d s2 = s3 / 2.0f + qc;
	const Eigen::Vector2d dp = (r3 + s2) / 2.0f;

	const Bezier b1(m_point1, r2, r3, dp);
	const Bezier b2(dp, s2, s3, m_point2);

	return {b1, b2};
}

Bezier::List Bezier::splitToConvex() const
{
	const InflexionPoints inflex = inflexions();
	const bool isReal1 = isRealInflexionPoint(inflex[0]);
	const bool isReal2 = isRealInflexionPoint(inflex[1]);

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
		double t1 = inflex[0].real();
		double t2 = inflex[1].real();

		const Pair splitted1 = split(t1);

		if (std::abs(t1 - t2) < 0.01) {
			return {splitted1[0], splitted1[1]};
		}
		else {
			// Order splitting points
			if (t1 > t2) {
				std::swap(t1, t2);
			}

			assert(t1 < t2);

			// Make t2 relative to second bezier.
			t2 = (1.0f - t1) * t2;

			const Pair splitted2 = splitted1[1].split(t2);

			return {splitted1[0], splitted2[0], splitted2[1]};
		}
	}

	// No split
	return {*this};
}

std::optional<Biarc> Bezier::toBiarc() const
{
	// First find V, second vertex of triangle.
	const std::optional<Eigen::Vector2d> optIntersection = ForwardLineIntersection(m_point1, m_control1, m_point2, m_control2);

	/* If the intersection is not forward (from direction P1 -> C1) or the tangents are parrallels,
	 * no biars can be computed.
	 */
	if (!optIntersection) {
		return std::nullopt;
	}

	const Eigen::Vector2d v = *optIntersection;

	// Find (G) incenter of triangle P1 V P2
	const Eigen::Vector2d incenter = TriangleIncenter(m_point1, v, m_point2);

	// Create biarc passing by P1 G P2 and with tangent at P1 and P2
	Biarc biarc(m_point1, incenter, m_point2, (m_control1 - m_point1), (m_control2 - m_point2));

	return std::make_optional(biarc);
}

Polyline Bezier::toLine() const
{
	return Polyline({Bulge{m_point1, m_point2, 0.0f}});
}

double Bezier::maxError(const Biarc &biarc) const
{
	const Eigen::Vector2d &middle = biarc.middle();
	const Eigen::Vector2d tangent = biarc.tangentAtMiddle();

	// Find nearest point on curve to biarc middle with same tangent.
	const Eigen::Vector2d nearest = findNearestPointWithTangent(middle, tangent, 0.001); // TODO const

	return (middle - nearest).squaredNorm();
}

}
