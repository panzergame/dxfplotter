#pragma once

#include <model/polyline.h>

#include <string>

namespace Model
{

class Path
{
private:
	Polyline m_polyline;
	std::string m_name;

public:
	explicit Path() = default;
	explicit Path(Polyline &&polyline, const std::string &name);

	Path &operator=(Path &&other) = default;

	const Polyline &polyline() const;
	const std::string &name() const;
};

using Paths = std::vector<Path>;
using PathsPtr = std::vector<Path *>;

Paths PathsFromPolylines(Polylines &&polylines);

}
