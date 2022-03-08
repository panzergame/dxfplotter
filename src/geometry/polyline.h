#pragma once

#include <common/aggregable.h>

#include <geometry/bulge.h>

#include <cavc/polylineoffset.hpp>

#include <serializer/access.h>

namespace geometry
{

class Polyline : public common::Aggregable<Polyline>
{
	friend serializer::Access<Polyline>;

private:
	Bulge::List m_bulges;

public:
	explicit Polyline() = default;
	explicit Polyline(Bulge::List &&bulges);
	explicit Polyline(const cavc::Polyline<double> &polyline);

	cavc::Polyline<double> toCavc() const;
	cavc::Polyline<double> toCavc(Orientation expectedOrientation) const;

	const QVector2D &start() const;
	QVector2D &start();
	const QVector2D &end() const;
	QVector2D &end();

	bool isClosed() const;
	bool isPoint() const;
	bool isLine() const;

	float length() const;

	Orientation orientation() const;

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

	Polyline::List offsetted(float margin) const;
	Polyline::List pocketted(const Polyline::List &islands, float margin) const;

	void transform(const QTransform &matrix);

	bool operator==(const Polyline &other) const;
};

}
