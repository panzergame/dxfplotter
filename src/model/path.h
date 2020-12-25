#pragma once

#include <geometry/polyline.h>

#include <common/aggregable.h>

#include <model/pathsettings.h>

#include <string>

#include <QObject>

namespace Model
{

class Path : public QObject, public Common::Aggregable<Path>
{
	Q_OBJECT;

private:
	Geometry::Polyline m_basePolyline;
	Geometry::Polyline::List m_offsetedPolylines;
	std::string m_name;
	PathSettings m_settings;

	struct {
		bool m_selected : 1;
		bool m_visible : 1;
	};

public:
	explicit Path(Geometry::Polyline &&basePolyline, const std::string &name, const PathSettings& settings);

	static ListPtr FromPolylines(Geometry::Polyline::List &&polylines, const PathSettings &settings);

	const Geometry::Polyline &basePolyline() const;
	const Geometry::Polyline::List &offsetedPolylines() const;
	Geometry::Polyline::List finalPolylines() const;
	const std::string &name() const;
	const PathSettings &settings() const;
	PathSettings &settings();

	void offset(float offset, float minimumPolylineLength, float minimumArcLength);
	void resetOffset();

	void setSelected(bool selected);
	void deselect();
	void toggleSelect();

	void setVisible(bool visible);

Q_SIGNALS:
	void selectedChanged(bool selected);
	void offseted();
};

}
