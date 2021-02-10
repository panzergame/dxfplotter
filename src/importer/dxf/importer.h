#pragma once

#include <importer/dxf/layer.h>

#include <geometry/polyline.h>
#include <geometry/bezier.h>

#include <string>
#include <unordered_map>

#include <libdxfrw/drw_entities.h>
#include <libdxfrw/drw_objects.h>

namespace Importer::Dxf
{

class Importer
{
private:
	Geometry::Polyline::List m_polylines;

	const float m_splineToArcPrecision;
	const float m_minimumSplineLength;

	std::unordered_map<std::string, Layer> m_nameToLayers;

	void addPolyline(const Geometry::Polyline &polyline);

	Geometry::Polyline bezierToPolyline(const Geometry::Bezier &rootBezier);

	void convertToPolylines(const DRW_Point &point);
	void convertToPolylines(const DRW_Line &line);
	void convertToPolylines(const DRW_Spline &spline);
	void convertToPolylines(const DRW_LWPolyline &lwpolyline);
	void convertToPolylines(const DRW_Circle &circle);
	void convertToPolylines(const DRW_Arc &arc);

	void addLayer(const DRW_Layer &layer);

public:
	explicit Importer(const std::string &filename, float splineToArcPrecision, float minimumSplineLength);

	Geometry::Polyline::List &&polylines();

	template <class Entity>
	void processEntity(const Entity &entity)
	{
		if (m_nameToLayers.at(entity.layer).visible()) {
			convertToPolylines(entity);
		}
	}
};

template <>
void Importer::processEntity(const DRW_Layer &layer);

}
