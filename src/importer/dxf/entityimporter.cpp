#include <importer/dxf/entityimporter.h>

namespace importer::dxf
{

BaseEntityImporter::BaseEntityImporter(Layer& layer, const Settings& settings)
	:m_layer(layer),
	m_settings(settings)
{
}

void BaseEntityImporter::addPolyline(const geometry::Polyline& polyline)
{
	m_layer.addPolyline(polyline);
}

}
