#include <path.h>
#include <layer.h>
#include <fmt/format.h>

#include <geometry/cleaner.h>

namespace Model
{

void Path::updateGlobalVisibility()
{
	const bool newGloballyVisible = visible() && m_layer.visible();
	if (m_globallyVisible != newGloballyVisible) {
		m_globallyVisible = newGloballyVisible;

		emit globalVisibilityChanged(m_globallyVisible);
	}
}

Path::Path(Geometry::Polyline &&basePolyline, const std::string &name, const PathSettings &settings, Layer &layer)
	:Renderable(name),
	m_basePolyline(basePolyline),
	m_settings(settings),
	m_layer(layer),
	m_globallyVisible(true)
{
	connect(&layer, &Layer::visibilityChanged, this, &Path::updateGlobalVisibility);
	connect(this, &Path::visibilityChanged, this, &Path::updateGlobalVisibility);
}

Path::ListUPtr Path::FromPolylines(Geometry::Polyline::List &&polylines, const PathSettings &settings, Layer &layer)
{
	const int size = polylines.size();
	Path::ListUPtr paths(size);

	for (int i = 0; i < size; ++i) {
		static const char *pathNameFormat = "({}) {}";
		const std::string pathName = fmt::format(pathNameFormat, layer.name(), i);
		paths[i].reset(new Path(std::move(polylines[i]), pathName, settings, layer));
	}

	return paths;
}

Layer &Path::layer()
{
	return m_layer;
}

const Layer &Path::layer() const
{
	return m_layer;
}

const Geometry::Polyline &Path::basePolyline() const
{
	return m_basePolyline;
}

Geometry::Polyline::List Path::finalPolylines() const
{
	return m_offsettedPath ? m_offsettedPath->offsettedPolylines() : Geometry::Polyline::List{m_basePolyline};
}

Model::OffsettedPath *Path::offsettedPath() const
{
	return m_offsettedPath.get();
}

void Path::offset(float margin, float minimumPolylineLength, float minimumArcLength)
{
	Geometry::Polyline::List offsettedPolylines = m_basePolyline.offsetted(margin);
	Geometry::Cleaner cleaner(std::move(offsettedPolylines), minimumPolylineLength, minimumArcLength);

	const OffsettedPath::Direction direction = (margin > 0.0f) ?
			OffsettedPath::Direction::LEFT : OffsettedPath::Direction::RIGHT;

	m_offsettedPath = std::make_unique<OffsettedPath>(cleaner.polylines(), direction);

	emit offsettedPathChanged();
}

void Path::resetOffset()
{
	m_offsettedPath.reset();

	emit offsettedPathChanged();
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

Geometry::CuttingDirection Path::cuttingDirection() const
{
	return (m_offsettedPath) ? m_offsettedPath->cuttingDirection() :
			 Geometry::CuttingDirection::FORWARD;
}

bool Path::globallyVisible() const
{
	return m_globallyVisible;
}

}

