#include <path.h>
#include <fmt/format.h>

#include <geometry/cleaner.h>

namespace Model
{

Path::Path(Geometry::Polyline &&basePolyline, const std::string &name, const PathSettings &settings)
	:Renderable(name),
	m_basePolyline(basePolyline),
	m_settings(settings)
{
}

Path::ListUPtr Path::FromPolylines(Geometry::Polyline::List &&polylines, const std::string& prefixName, const PathSettings &settings)
{
	const int size = polylines.size();
	Path::ListUPtr paths(size);

	for (int i = 0; i < size; ++i) {
		static const char *pathNameFormat = "({}) {}";
		const std::string pathName = fmt::format(pathNameFormat, prefixName, i);
		paths[i].reset(new Path(std::move(polylines[i]), pathName, settings));
	}

	return paths;
}

const Geometry::Polyline &Path::basePolyline() const
{
	return m_basePolyline;
}

Geometry::Polyline::List Path::finalPolylines() const
{
	if (m_offsetedPolylines.empty()) {
		return {m_basePolyline};
	}
	return m_offsetedPolylines;
}

const Geometry::Polyline::List &Path::offsetedPolylines() const
{
	return m_offsetedPolylines;
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

bool Path::isPoint() const
{
	return m_basePolyline.isPoint();
}

const Model::PathSettings &Path::settings() const
{
	return m_settings;
}

Model::PathSettings &Path::settings()
{
	return m_settings;
}

}

