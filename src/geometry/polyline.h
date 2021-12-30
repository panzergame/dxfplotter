#pragma once

#include <common/aggregable.h>

#include <geometry/bulge.h>

#include <cavc/polylineoffset.hpp>

#include <serializer/access.h>

namespace geometry
{

class Polyline : public Common::Aggregable<Polyline>
{
	friend serializer::Access<Polyline>;

private:
	Bulge::List m_bulges;

	explicit Polyline(const cavc::Polyline<double> &polyline);

public:
	explicit Polyline() = default;
	explicit Polyline(Bulge::List &&bulges);

	const Eigen::Vector2d &start() const;
	Eigen::Vector2d &start();
	const Eigen::Vector2d &end() const;
	Eigen::Vector2d &end();

	bool isClosed() const;
	bool isPoint() const;

	double length() const;

	Polyline &invert();
	Polyline inverse() const;

	Polyline& operator+=(const Polyline &other);

	template <class Functor>
	void forEachBulge(Functor &&functor) const
	{
		for (const Bulge &bulge : m_bulges) {
			functor(bulge);
		}
	}

	template <class Functor>
	void transformBulge(Functor &&functor)
	{
		for (Bulge &bulge : m_bulges) {
			functor(bulge);
		}
	}

	Polyline::List offsetted(double offset) const;

	void transform(const Eigen::Affine2d &matrix);

	bool operator==(const Polyline &other) const;
};

}
