#pragma once

#include <common/aggregable.h>

#include <geometry/bulge.h>

#include <cavc/polylineoffset.hpp>

#include <serializer/access.h>

namespace Geometry
{

class Polyline : public Common::Aggregable<Polyline>
{
	friend Serializer::Access<Polyline>;

private:
	Bulge::List m_bulges;

	explicit Polyline(const cavc::Polyline<double> &polyline);

public:
	explicit Polyline() = default;
	explicit Polyline(Bulge::List &&bulges);

	const QVector2D &start() const;
	QVector2D &start();
	const QVector2D &end() const;
	QVector2D &end();

	bool isClosed() const;
	bool isPoint() const;

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

	Polyline::List offsetted(float offset) const;
};

}
