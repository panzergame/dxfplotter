module;

#include <string>
#include <unordered_map>
#include <vector>
#include <libdxfrw/drw_entities.h>
#include <libdxfrw/drw_objects.h>

export module importer.dxf.importer;

import common.exception;
import geometry.polyline;
import importer.dxf.entityimporter;
import importer.dxf.layer;
import importer.dxf.utils;

export namespace importer::dxf
{

class Importer
{
private:
	const BaseEntityImporter::Settings m_entityImporterSettings;

	std::unordered_map<std::string, Layer> m_nameToLayers;
	bool m_ignoreEntities;

	void addLayer(const DRW_Layer& layer);

public:
	explicit Importer(const std::string& filename, float splineToArcPrecision, float minimumSplineLength,
					  float minimumArcLength);

	Layer::List layers() const;

	template<class Entity>
	void processEntity(const Entity& entity)
	{
		if (!m_ignoreEntities) {
			auto it = m_nameToLayers.find(entity.layer);
			if (it != m_nameToLayers.end()) {
				Layer& layer = it->second;

				EntityImporter<Entity> entityImporter(layer, m_entityImporterSettings);
				entityImporter(entity);
			}
		}
	}

	void startBlock();
	void endBlock();
};

template<>
void Importer::processEntity(const DRW_Layer& layer);

}
