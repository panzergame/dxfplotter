#pragma once

#include <geometry/polyline.h>

#include <string>

namespace Importer::Dxf
{

class Importer
{
	friend void operator<<(Importer &importer, const Geometry::Polyline::List &polylines);

private:
	Geometry::Polyline::List m_polylines;

public:
	explicit Importer(const std::string &filename);

	Geometry::Polyline::List &&polylines();
};

void operator<<(Importer &importer, const Geometry::Polyline::List &polylines);

}
