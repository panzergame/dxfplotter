#pragma once

#include <common/aggregable.h>

#include <complex>

#include <geometry/biarc.h>

namespace geometry
{

class Bezier : public Common::Aggregable<Bezier>
{
private:
	using Complex = std::complex<double>;
	using InflexionPoints = std::array<Complex, 2>;

	Eigen::Vector2d m_point1;
	Eigen::Vector2d m_point2;
	Eigen::Vector2d m_control1;
	Eigen::Vector2d m_control2;

	static bool isRealInflexionPoint(const Bezier::Complex &point);

	InflexionPoints inflexions() const;
	Eigen::Vector2d derivativeAt(double t) const;

	Eigen::Vector2d findNearestPointWithTangent(const Eigen::Vector2d &point, const Eigen::Vector2d& tangent, double maxError) const;

public:
	explicit Bezier(const Eigen::Vector2d &p1, const Eigen::Vector2d &c1,
			const Eigen::Vector2d &c2, const Eigen::Vector2d &p2);
	Bezier() = default;

	const Eigen::Vector2d &point1() const;
	const Eigen::Vector2d &point2() const;
	const Eigen::Vector2d &control1() const;
	const Eigen::Vector2d &control2() const;

	Eigen::Vector2d at(double t) const;

	double approximateLength() const;

	Pair split(double t) const;
	Pair splitHalf() const;
	/// Split bezier keeping only convex shape.
	List splitToConvex() const;

	std::optional<Biarc> toBiarc() const;
	Polyline toLine() const;

	double maxError(const Biarc &biarc) const;
};

}
