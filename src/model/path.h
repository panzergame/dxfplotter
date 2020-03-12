#pragma once

#include <geometry/polyline.h>

#include <common/aggregable.h>

#include <string>

#include <QObject>

namespace Model
{

class Path : public QObject, public Common::Aggregable<Path>
{
	Q_OBJECT;

public:
	class Settings
	{
	private:
		float m_feedRate;
		float m_intensity;

	public:
		explicit Settings() = default;
		explicit Settings(float feedRate, float intensity);

		float feedRate() const;
		void setFeedRate(float feedRate);

		float intensity() const;
		void setIntensity(float intensity);
	};

private:
	Geometry::Polyline m_polyline;
	std::string m_name;
	Settings m_settings;

public:
	explicit Path(Geometry::Polyline &&polyline, const std::string &name, const Settings& settings);

	const Geometry::Polyline &polyline() const;
	const std::string &name() const;
	const Settings &settings() const;

	void select();
	void deselect();

Q_SIGNALS:
	void selected();
	void deselected();
};

Path::ListPtr PathsFromPolylines(Geometry::Polyline::List &&polylines, const Path::Settings &settings); // TODO move in class

}
