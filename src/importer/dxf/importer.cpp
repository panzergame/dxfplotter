#include <importer.h>
#include <interface.h>

#include <libdxfrw/libdxfrw.h>

namespace Importer::Dxf
{

Importer::Importer(const std::string& filename)
{
	Interface interface(*this);

	dxfRW rw(filename.c_str());
	rw.read(&interface, false);
}

Geometry::Polylines &&Importer::polylines()
{
	return std::move(m_polylines);
}

void operator<<(Importer &importer, const Geometry::Polylines &polylines)
{
	importer.m_polylines.insert(importer.m_polylines.begin(), polylines.begin(), polylines.end());
}

}
