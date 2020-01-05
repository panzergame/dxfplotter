#pragma once

#include <core/bulge.h>

#include <vector>

namespace Core
{

class Polyline
{
friend std::ostream &operator<<(std::ostream &stream, const Polyline &polyline);

private:
	Bulges m_bulges;

public:
	explicit Polyline() = default;
	explicit Polyline(Bulges &&bulges);

	const QVector2D &start() const;
	const QVector2D &end() const;

	Polyline &invert();

	Polyline& operator+=(const Polyline &other);
};

using Polylines = std::vector<Polyline>;

std::ostream &operator<<(std::ostream &stream, const Polyline &polyline);

}
