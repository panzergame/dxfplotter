#pragma once

#include <geometry/polyline.h>

#include <common/aggregable.h>

#include <string>

namespace Model
{

class Path : public Common::Aggregable<Path>
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

Path::List PathsFromPolylines(Geometry::Polyline::List &&polylines);

}
