#pragma once

#include <geometry/bulge.h>

#include <vector>

namespace Geometry
{

class Polyline
{
// friend std::ostream &operator<<(std::ostream &stream, const Polyline &polyline); TODO qInfo

private:
	Bulges m_bulges;

public:
	explicit Polyline() = default;
	explicit Polyline(Bulges &&bulges);

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
};

using Polylines = std::vector<Polyline>;

// std::ostream &operator<<(std::ostream &stream, const Polyline &polyline);

}
