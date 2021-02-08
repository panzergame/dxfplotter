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
	Geometry::Polyline::List finalPolylines() const;

	const Geometry::Polyline::List &offsetedPolylines() const;
	void offset(float offset, float minimumPolylineLength, float minimumArcLength);
	void resetOffset();

	bool isPoint() const;

	const std::string &name() const;

	const PathSettings &settings() const;
	PathSettings &settings();

	bool visible() const;
	void setVisible(bool visible);
	void toggleVisible();

	void setSelected(bool selected);
	void deselect();
	void toggleSelect();

Q_SIGNALS:
	void selectedChanged(bool selected);
	void offseted();
};

}
