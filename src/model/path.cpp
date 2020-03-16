#include <path.h>

namespace Model
{

Path::Path(Geometry::Polyline &&polyline, const std::string &name, const PathSettings &settings)
	:m_polyline(polyline),
	m_name(name),
	m_settings(settings),
	m_selected(false)
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

const Model::PathSettings &Path::settings() const
{
	return m_settings;
}

Model::PathSettings &Path::settings()
{
	return m_settings;
}

void Path::select()
{
	if (!m_selected) {
		m_selected = true;

		emit selected();
	}
}

void Path::deselect()
{
	if (m_selected) {
		m_selected = false;

		emit deselected();
	}
}

void Path::toggleSelect()
{
	if (m_selected) {
		deselect();
	}
	else {
		select();
	}
}

Path::ListPtr PathsFromPolylines(Geometry::Polyline::List &&polylines, const PathSettings &settings)
{
	const int size = polylines.size();
	Path::ListPtr paths(size);

	for (int i = 0; i < size; ++i) {
		paths[i] = new Path(std::move(polylines[i]), "Path " + std::to_string(i), settings);
	}

	return paths;
}

}
