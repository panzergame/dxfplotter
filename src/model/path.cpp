#include <path.h>
#include <layer.h>
#include <fmt/format.h>

#include <geometry/cleaner.h>
#include <geometry/pocketer.h>

namespace model
{

void Path::updateGlobalVisibility()
{
	const bool newGloballyVisible = visible() && m_layer->visible();
	if (m_globallyVisible != newGloballyVisible) {
		m_globallyVisible = newGloballyVisible;

		emit globalVisibilityChanged(m_globallyVisible);
	}
}

Path::Path(geometry::Polyline &&basePolyline, const std::string &name, const PathSettings &settings)
	:Renderable(name),
	m_basePolyline(basePolyline),
	m_settings(settings),
	m_globallyVisible(true)
{
	connect(this, &Path::visibilityChanged, this, &Path::updateGlobalVisibility);
}

Path::ListUPtr Path::FromPolylines(geometry::Polyline::List &&polylines, const PathSettings &settings, const std::string &layerName)
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

const geometry::Polyline &Path::basePolyline() const
{
	return m_basePolyline;
}

geometry::Polyline::List Path::finalPolylines() const
{
	return m_offsettedPath ? m_offsettedPath->polylines() : geometry::Polyline::List{m_basePolyline};
}

model::OffsettedPath *Path::offsettedPath() const
{
	return m_offsettedPath.get();
}

void Path::offset(float margin, float minimumPolylineLength, float minimumArcLength)
{
	geometry::Polyline::List offsettedPolylines = m_basePolyline.offsetted(margin);
	geometry::Cleaner cleaner(std::move(offsettedPolylines), minimumPolylineLength, minimumArcLength);

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

void Path::pocket(const Path::ListCPtr &islands, float scaledRadius, float minimumPolylineLength, float minimumArcLength)
{
	geometry::Polyline::ListCPtr polylineIslands(islands.size());
	std::transform(islands.begin(), islands.end(), polylineIslands.begin(), [](const Path *path){
		return &path->basePolyline();
	});

	geometry::Pocketer pocketer(m_basePolyline, polylineIslands, scaledRadius, minimumPolylineLength);
	geometry::Cleaner cleaner(std::move(pocketer.polylines()), minimumPolylineLength, minimumArcLength);

	// Pocket is left when polyline is CCW winded.
	static const OffsettedPath::Direction basePolylineOrientationToPocketDirection[] = {
		OffsettedPath::Direction::RIGHT, // Orientation::CW
		OffsettedPath::Direction::LEFT // Orientation::CCW
	};
	const OffsettedPath::Direction direction = basePolylineOrientationToPocketDirection[static_cast<int>(pocketer.borderOrientation())];

	m_offsettedPath = std::make_unique<OffsettedPath>(cleaner.polylines(), direction);

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

geometry::Rect Path::boundingRect() const
{
	return (m_offsettedPath) ? m_offsettedPath->boundingRect() :
			 m_basePolyline.boundingRect();
}

bool Path::isPoint() const
{
	return m_basePolyline.isPoint();
}

const model::PathSettings &Path::settings() const
{
	return m_settings;
}

model::PathSettings &Path::settings()
{
	return m_settings;
}

geometry::CuttingDirection Path::cuttingDirection() const
{
	return (m_offsettedPath) ? m_offsettedPath->cuttingDirection() :
			 geometry::CuttingDirection::FORWARD;
}

bool Path::globallyVisible() const
{
	return m_globallyVisible;
}

}

