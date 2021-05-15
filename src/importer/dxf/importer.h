#pragma once

#include <importer/dxf/entityimporter.h>
#include <importer/dxf/layer.h>

#include <geometry/polyline.h>

#include <string>
#include <unordered_map>

#include <libdxfrw/drw_entities.h>
#include <libdxfrw/drw_objects.h>

namespace Importer::Dxf
{

class Importer
{
private:
	const BaseEntityImporter::Settings m_entityImporterSettings;

	std::unordered_map<std::string, Layer> m_nameToLayers;

	void addLayer(const DRW_Layer &layer);

public:
	explicit Importer(const std::string &filename, float splineToArcPrecision, float minimumSplineLength);

	Layer::List layers() const;

	template <class Entity>
	void processEntity(const Entity &entity)
	{
		auto it = m_nameToLayers.find(entity.layer);
		if (it != m_nameToLayers.end()) {
			Layer &layer = it->second;

			EntityImporter<Entity> entityImporter(layer, m_entityImporterSettings);
			entityImporter(entity);
		}
	}
};

template <>
void Importer::processEntity(const DRW_Layer &layer);

}
