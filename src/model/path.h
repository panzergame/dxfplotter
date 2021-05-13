#pragma once

#include <geometry/polyline.h>

#include <common/aggregable.h>

#include <model/renderable.h>
#include <model/pathsettings.h>

#include <string>

namespace Model
{

class Layer;

class Path : public Renderable, public Common::Aggregable<Path>
{
	Q_OBJECT;

private:
	Geometry::Polyline m_basePolyline;
	Geometry::Polyline::List m_offsetedPolylines;
	PathSettings m_settings;
	Layer &m_layer;
	bool m_globallyVisible;

	void updateGlobalVisibility();

public:
	explicit Path(Geometry::Polyline &&basePolyline, const std::string &name, const PathSettings& settings, Layer &layer);

	static ListUPtr FromPolylines(Geometry::Polyline::List &&polylines, const PathSettings &settings, Layer &layer);

	Layer &layer();
	const Layer &layer() const;

	const Geometry::Polyline &basePolyline() const;
	Geometry::Polyline::List finalPolylines() const;

	const Geometry::Polyline::List &offsetedPolylines() const;
	void offset(float offset, float minimumPolylineLength, float minimumArcLength);
	void resetOffset();

	bool isPoint() const;

	const PathSettings &settings() const;
	PathSettings &settings();

	bool globallyVisible() const;

Q_SIGNALS:
	void globalVisibilityChanged(bool globallyVisible);
	void offseted();
};

}
