#pragma once

#include <common/aggregable.h>

#include <QVector2D>

#include <complex>

#include <geometry/biarc.h>

namespace geometry
{

class Bezier : public common::Aggregable<Bezier>
{
private:
	using Complex = std::complex<float>;
	using InflexionPoints = std::array<Complex, 2>;

	QVector2D m_point1;
	QVector2D m_point2;
	QVector2D m_control1;
	QVector2D m_control2;

	static bool isRealInflexionPoint(const Bezier::Complex &point);

	InflexionPoints inflexions() const;
	QVector2D derivativeAt(float t) const;

	QVector2D findNearestPointWithTangent(const QVector2D &point, const QVector2D& tangent, float maxError) const;

public:
	explicit Bezier(const QVector2D &p1, const QVector2D &c1,
			const QVector2D &c2, const QVector2D &p2);
	Bezier() = default;

	const QVector2D &point1() const;
	const QVector2D &point2() const;
	const QVector2D &control1() const;
	const QVector2D &control2() const;

	QVector2D at(float t) const;

	float approximateLength() const;

	Pair split(float t) const;
	Pair splitHalf() const;
	/// Split bezier keeping only convex shape.
	List splitToConvex() const;

	std::optional<Biarc> toBiarc() const;
	Polyline toLine() const;

	bool isPoint() const;

	float maxError(const Biarc &biarc) const;
};

}
