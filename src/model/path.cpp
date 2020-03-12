#include <path.h>

namespace Model
{

Path::Settings::Settings(float feedRate, float intensity)
	:m_feedRate(feedRate),
	m_intensity(intensity)
{
}

float Path::Settings::feedRate() const
{
	return m_feedRate;
}

void Path::Settings::setFeedRate(float feedRate)
{
	m_feedRate = feedRate;
}

float Path::Settings::intensity() const
{
	return m_intensity;
}

void Path::Settings::setIntensity(float intensity)
{
	m_intensity = intensity;
}


Path::Path(Geometry::Polyline &&polyline, const std::string &name, const Settings &settings)
	:m_polyline(polyline),
	m_name(name),
	m_settings(settings)
{
}

const Geometry::Polyline &Path::polyline() const
{
	return m_polyline;
}

const std::string &Path::name() const
{
	return m_name;
}

const Model::Path::Settings &Path::settings() const
{
	return m_settings;
}

void Path::select()
{
	emit selected();
}

void Path::deselect()
{
	emit deselected();
}

Path::ListPtr PathsFromPolylines(Geometry::Polyline::List &&polylines, const Path::Settings &settings)
{
	const int size = polylines.size();
	Path::ListPtr paths(size);

	for (int i = 0; i < size; ++i) {
		paths[i] = new Path(std::move(polylines[i]), "Path " + std::to_string(i), settings);
	}

	return paths;
}

}
