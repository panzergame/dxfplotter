#include <importer.h>
#include <interface.h>

#include <common/exception.h>

#include <libdxfrw/libdxfrw.h>

#include <QDebug> // TODO

namespace Importer::Dxf
{

void Importer::addLayer(const DRW_Layer &layer)
{
	if (layer.plotF) {
		const std::string &name = layer.name;
		m_nameToLayers.emplace(name, Layer(name));
	}
}

Importer::Importer(const std::string& filename, float splineToArcPrecision, float minimumSplineLength)
	:m_entityImporterSettings({splineToArcPrecision, minimumSplineLength})
{
	Interface interface(*this);

	dxfRW rw(filename.c_str());
	if (!rw.read(&interface, false)) {
		throw Common::FileException();
	}
}

Layer::List Importer::layers()
{
	Layer::List layers(m_nameToLayers.size());
	std::transform(m_nameToLayers.begin(), m_nameToLayers.end(), layers.begin(),
		[](const auto& pair){ return pair.second; });

	return layers;
}

template <>
void Importer::processEntity(const DRW_Layer &layer)
{
	addLayer(layer);
}

}
