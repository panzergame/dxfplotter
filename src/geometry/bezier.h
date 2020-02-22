#pragma once

#include <QVector2D>

#include <complex>

namespace Geometry
{

class Bezier
{
private:
	using Complex = std::complex<float>;

	QVector2D m_point1;
	QVector2D m_point2;
	QVector2D m_control1;
	QVector2D m_control2;

	static bool isRealInflexionPoint(const Bezier::Complex &point);

public:
	using InflexionPoints = std::array<Complex, 2>;
	using Pair = std::array<Bezier, 2>;
	using List = std::vector<Bezier>;

	explicit Bezier(const QVector2D &p1, const QVector2D &c1,
			const QVector2D &c2, const QVector2D &p2);
	Bezier() = default;

	const QVector2D &point1() const;
	const QVector2D &point2() const;
	const QVector2D &control1() const;
	const QVector2D &control2() const;

	InflexionPoints inflexions() const;
	Pair split(float t) const;
	Pair splitHalf() const;
	QVector2D at(float t) const;

	/// Split bezier keeping only convex shape.
	List splitToConvex() const;

	QVector2D incenter() const;
};

// using Beziers = std::vector<Bezier>; // TODO toujours faire Bezier::List pour éviter le problème comme Pair

}
