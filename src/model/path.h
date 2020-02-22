#pragma once

#include <geometry/polyline.h>

#include <string>

namespace Model
{

class Path
{
private:
	Geometry::Polyline m_polyline;
	std::string m_name;

public:
	explicit Path() = default;
	explicit Path(Geometry::Polyline &&polyline, const std::string &name);

	Path &operator=(Path &&other) = default;

	const Geometry::Polyline &polyline() const;
	const std::string &name() const;
};

using Paths = std::vector<Path>;
using PathsPtr = std::vector<Path *>;

Paths PathsFromPolylines(Geometry::Polylines &&polylines);

}
