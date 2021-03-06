#pragma once

#include <geometry/polyline.h>

#include <common/aggregable.h>

#include <model/renderable.h>
#include <model/pathsettings.h>
#include <model/offsettedpath.h>

#include <string>

namespace Model
{

class Layer;

class Path : public Renderable, public Common::Aggregable<Path>
{
	Q_OBJECT;

private:
	Geometry::Polyline m_basePolyline;
	std::unique_ptr<Model::OffsettedPath> m_offsettedPath;
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

	Model::OffsettedPath *offsettedPath() const;
	void offset(float margin, float minimumPolylineLength, float minimumArcLength);
	void resetOffset();

	bool isPoint() const;

	const PathSettings &settings() const;
	PathSettings &settings();

	Geometry::CuttingDirection cuttingDirection() const;

	bool globallyVisible() const;

Q_SIGNALS:
	void globalVisibilityChanged(bool globallyVisible);
	void offsettedPathChanged();
};

}
