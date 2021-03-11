#include <importer/dxf/entityimporter.h>

namespace Importer::Dxf
{

BaseEntityImporter::BaseEntityImporter(Geometry::Layer& layer, const Settings& settings)
	:m_layer(layer),
	m_settings(settings)
{
}

void BaseEntityImporter::addPolyline(const Geometry::Polyline& polyline)
{
	m_layer.addPolyline(polyline);
}

}
