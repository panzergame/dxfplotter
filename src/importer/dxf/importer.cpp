#include <importer.h>
#include <interface.h>

#include <common/exception.h>

#include <libdxfrw/libdxfrw.h>

namespace importer::dxf
{

void Importer::addLayer(const DRW_Layer &layer)
{
	if (layer.plotF) {
		const std::string &name = layer.name;
		m_nameToLayers.emplace(name, Layer(name));
	}
}

Importer::Importer(float splineToArcPrecision, float minimumSplineLength, float minimumArcLength)
	:m_entityImporterSettings({splineToArcPrecision, minimumSplineLength, minimumArcLength}),
	m_ignoreEntities(false)
{
}

bool Importer::import(std::istream& stream)
{
	Interface interface(*this);

	dxfRW rw("");
	return rw.read(stream, &interface, false);
}

Layer::List Importer::layers() const
{
	Layer::List layers(m_nameToLayers.size());
	std::transform(m_nameToLayers.begin(), m_nameToLayers.end(), layers.begin(),
		[](const auto& pair){ return pair.second; });

	return layers;
}

void Importer::startBlock()
{
	m_ignoreEntities = true;
}

void Importer::endBlock()
{
	m_ignoreEntities = false;
}

template <>
void Importer::processEntity(const DRW_Layer &layer)
{
	addLayer(layer);
}

}
