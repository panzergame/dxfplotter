#include <path.h>
#include <layer.h>
#include <fmt/format.h>

#include <geometry/cleaner.h>

namespace Model
{

void Path::updateGlobalVisibility()
{
	const bool newGloballyVisible = visible() && m_layer->visible();
	if (m_globallyVisible != newGloballyVisible) {
		m_globallyVisible = newGloballyVisible;

		emit globalVisibilityChanged(m_globallyVisible);
	}
}

Path::Path(Geometry::Polyline &&basePolyline, const std::string &name, const PathSettings &settings)
	:Renderable(name),
	m_basePolyline(basePolyline),
	m_settings(settings),
	m_globallyVisible(true)
{
	connect(this, &Path::visibilityChanged, this, &Path::updateGlobalVisibility);
}

Path::ListUPtr Path::FromPolylines(Geometry::Polyline::List &&polylines, const PathSettings &settings, const std::string &layerName)
{
	const int size = polylines.size();
	Path::ListUPtr paths(size);

	for (int i = 0; i < size; ++i) {
		static const char *pathNameFormat = "({}) {}";
		const std::string pathName = fmt::format(pathNameFormat, layerName, i);
		paths[i].reset(new Path(std::move(polylines[i]), pathName, settings));
	}

	return paths;
}

Layer &Path::layer()
{
	return *m_layer;
}

const Layer &Path::layer() const
{
	return *m_layer;
}

void Path::setLayer(Layer &layer)
{
	m_layer = &layer;
	updateGlobalVisibility();
	connect(m_layer, &Layer::visibilityChanged, this, &Path::updateGlobalVisibility);
}

const Geometry::Polyline &Path::basePolyline() const
{
	return m_basePolyline;
}

Geometry::Polyline::List Path::finalPolylines() const
{
	return m_offsettedPath ? m_offsettedPath->polylines() : Geometry::Polyline::List{m_basePolyline};
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

void Path::transform(const QTransform &matrix)
{
	m_basePolyline.transform(matrix);
	emit basePolylineTransformed();

	if (m_offsettedPath) {
		m_offsettedPath->transform(matrix);
	}
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

