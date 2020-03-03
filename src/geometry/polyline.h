#pragma once

#include <common/aggregable.h>

#include <geometry/bulge.h>

#include <vector>

namespace Geometry
{

class Polyline : public Common::Aggregable<Polyline>
{
// friend std::ostream &operator<<(std::ostream &stream, const Polyline &polyline); TODO qInfo

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
	void travelAlong(Functor &functor) const
	{
		for (const Bulge &bulge : m_bulges) {
			functor(bulge);
		}
	}

	template <class Functor>
	void travelAlong(Functor &&functor) const
	{
		for (const Bulge &bulge : m_bulges) {
			functor(bulge);
		}
	}
};

// std::ostream &operator<<(std::ostream &stream, const Polyline &polyline);

}
