module;

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

#include <libdxfrw/libdxfrw.h>
#include <libdxfrw/drw_entities.h>
#include <libdxfrw/drw_objects.h>

module importer.dxf.importer;

import common.exception;
import geometry.polyline;
import importer.dxf.entityimporter;
import importer.dxf.interface;
import importer.dxf.layer;
import importer.dxf.utils;
namespace importer::dxf
{

void Importer::addLayer(const DRW_Layer& layer)
{
	if (layer.plotF) {
		const std::string& name = layer.name;
		m_nameToLayers.emplace(name, Layer(name));
	}
}

Importer::Importer(const std::string& filename, float splineToArcPrecision, float minimumSplineLength,
				   float minimumArcLength)
	: m_entityImporterSettings({ splineToArcPrecision, minimumSplineLength, minimumArcLength })
	, m_ignoreEntities(false)
{
	Interface interface(*this);

	dxfRW rw(filename.c_str());
	if (!rw.read(&interface, false)) {
		throw common::FileCouldNotOpenException();
	}
}

Layer::List Importer::layers() const
{
	Layer::List layers(m_nameToLayers.size());
	std::transform(m_nameToLayers.begin(), m_nameToLayers.end(), layers.begin(), [](const auto& pair) {
		return pair.second;
	});

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

template<>
void Importer::processEntity(const DRW_Layer& layer)
{
	addLayer(layer);
}

}
