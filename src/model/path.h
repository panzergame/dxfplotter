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
	Geometry::Polyline m_polyline;
	std::string m_name;
	PathSettings m_settings;
	bool m_selected;

public:
	explicit Path(Geometry::Polyline &&polyline, const std::string &name, const PathSettings& settings);

	const Geometry::Polyline &polyline() const;
	const std::string &name() const;
	const PathSettings &settings() const;
	PathSettings &settings();

	void select();
	void deselect();
	void toggleSelect();

Q_SIGNALS:
	void selected();
	void deselected();
};

Path::ListPtr PathsFromPolylines(Geometry::Polyline::List &&polylines, const PathSettings &settings); // TODO move in class

}
