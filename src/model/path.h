#pragma once

#include <geometry/polyline.h>

#include <common/aggregable.h>

#include <model/renderable.h>
#include <model/pathsettings.h>

#include <string>

namespace Model
{

class Path : public Renderable, public Common::Aggregable<Path>
{
	Q_OBJECT;

private:
	Geometry::Polyline m_basePolyline;
	Geometry::Polyline::List m_offsetedPolylines;
	PathSettings m_settings;

public:
	explicit Path(Geometry::Polyline &&basePolyline, const std::string &name, const PathSettings& settings);

	static ListUPtr FromPolylines(Geometry::Polyline::List &&polylines, const std::string& prefixName, const PathSettings &settings);

	const Geometry::Polyline &basePolyline() const;
	Geometry::Polyline::List finalPolylines() const;

	const Geometry::Polyline::List &offsetedPolylines() const;
	void offset(float offset, float minimumPolylineLength, float minimumArcLength);
	void resetOffset();

	bool isPoint() const;

	const PathSettings &settings() const;
	PathSettings &settings();

Q_SIGNALS:
	void offseted();
};

}
