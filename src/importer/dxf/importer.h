#pragma once

#include <model/polyline.h>

#include <string>

namespace Importer::Dxf
{

class Importer
{
	friend void operator<<(Importer &importer, const Model::Polylines &polylines);

private:
	Model::Polylines m_polylines;

public:
	explicit Importer(const std::string &filename);

	Model::Polylines &&polylines();
};

void operator<<(Importer &importer, const Model::Polylines &polylines);

}
