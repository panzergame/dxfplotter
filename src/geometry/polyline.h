#pragma once

#include <common/aggregable.h>

#include <geometry/bulge.h>

#include <vector>

namespace Geometry
{

class Polyline : public Common::Aggregable<Polyline>
{
private:
	Bulge::List m_bulges;

public:
	explicit Polyline() = default;
	explicit Polyline(Bulge::List &&bulges);

	const QVector2D &start() const;
	const QVector2D &end() const;

	Polyline &invert();

	Polyline& operator+=(const Polyline &other);

	template <class Functor>
	void forEachBulge(Functor &&functor) const
	{
		for (const Bulge &bulge : m_bulges) {
			functor(bulge);
		}
	}

	template <class Functor>
	void transformEachBulge(Functor &&functor)
	{
		for (Bulge &bulge : m_bulges) {
			functor(bulge);
		}
	}
};

}
