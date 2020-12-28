#include <path.h>

#include <geometry/cleaner.h>

namespace Model
{

Path::Path(Geometry::Polyline &&basePolyline, const std::string &name, const PathSettings &settings)
	:m_basePolyline(basePolyline),
	m_name(name),
	m_settings(settings),
	m_selected(false),
	m_visible(true)
{
}

Path::ListPtr Path::FromPolylines(Geometry::Polyline::List &&polylines, const PathSettings &settings)
{
	const int size = polylines.size();
	Path::ListPtr paths(size);

	for (int i = 0; i < size; ++i) {
		paths[i] = new Path(std::move(polylines[i]), "Path " + std::to_string(i), settings);
	}

	return paths;
}

const Geometry::Polyline &Path::basePolyline() const
{
	return m_basePolyline;
}

const Geometry::Polyline::List &Path::offsetedPolylines() const
{
	return m_offsetedPolylines;
}

Geometry::Polyline::List Path::finalPolylines() const
{
	if (m_offsetedPolylines.empty()) {
		return {m_basePolyline};
	}
	return m_offsetedPolylines;
}

const std::string &Path::name() const
{
	return m_name;
}

const Model::PathSettings &Path::settings() const
{
	return m_settings;
}

Model::PathSettings &Path::settings()
{
	return m_settings;
}

void Path::offset(float offset, float minimumPolylineLength, float minimumArcLength)
{
	Geometry::Polyline::List offsetedPolylines = m_basePolyline.offsetted(offset);

	Geometry::Cleaner cleaner(std::move(offsetedPolylines), minimumPolylineLength, minimumArcLength);

	m_offsetedPolylines = cleaner.polylines();

	emit offseted();
}

void Path::resetOffset()
{
	m_offsetedPolylines.clear();

	emit offseted();
}

void Path::setSelected(bool selected)
{
	if (m_selected != selected) {
		m_selected = selected;

		emit selectedChanged(m_selected);
	}
}

void Path::toggleSelect()
{
	setSelected(!m_selected);
}

}

