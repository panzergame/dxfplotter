#pragma once

#include <geometry/polyline.h>

#include <common/aggregable.h>

#include <string>

namespace Model
{

class Path : public Common::Aggregable<Path>
{
public:
	class Settings
	{
	private:
		float m_feedRate;

	public:
		explicit Settings() = default;
		explicit Settings(float feedRate);

		float feedRate() const;
		void setFeedRate(float feedRate);
	};

private:
	Geometry::Polyline m_polyline;
	std::string m_name;
	Settings m_settings;

public:
	explicit Path() = default;
	explicit Path(Geometry::Polyline &&polyline, const std::string &name, const Settings& settings);

	Path &operator=(Path &&other) = default;

	const Geometry::Polyline &polyline() const;
	const std::string &name() const;
	const Settings &settings() const;
};

Path::List PathsFromPolylines(Geometry::Polyline::List &&polylines, const Path::Settings &settings); // TODO move in class

}
