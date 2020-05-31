#pragma once

#include <geometry/polyline.h>
#include <geometry/bezier.h>

#include <string>

#include <libdxfrw/drw_entities.h>

namespace Importer::Dxf
{

class Importer
{
private:
	Geometry::Polyline::List m_polylines;

	const float m_splineToArcPrecision;
	const float m_minimumSplineLength;

	void addPolyline(const Geometry::Polyline &polyline);

	Geometry::Polyline bezierToPolyline(const Geometry::Bezier &rootBezier);

public:
	explicit Importer(const std::string &filename, float splineToArcPrecision, float minimumSplineLength);

	Geometry::Polyline::List &&polylines();

	void convertToPolylines(const DRW_Line &line);
	void convertToPolylines(const DRW_Spline &spline);
	void convertToPolylines(const DRW_LWPolyline &lwpolyline);
	void convertToPolylines(const DRW_Circle &circle);
	void convertToPolylines(const DRW_Arc &arc);
};

}
