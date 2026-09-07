module;

#include <cstdint>
#include <serializer/access.h>
#include <fmt/format.h>
#include <QObject>
#include <QTransform>
#include <QtCore/qtmochelpers.h>

export module model.path;

import common.aggregable;
import geometry.bulge;
import geometry.filter.cleaner;
import geometry.pocketer;
import geometry.polyline;
import geometry.rect;
import geometry.utils;
import model.offsettedpath;
import model.pathsettings;
import model.renderable;
import common.copy;
export namespace model
{

class Layer;

class Path : public Renderable, public common::Aggregable<Path>
{
	Q_OBJECT;

	friend serializer::Access<Path>;

private:
	geometry::Polyline m_basePolyline;
	std::unique_ptr<model::OffsettedPath> m_offsettedPath;
	PathSettings m_settings;
	Layer* m_layer;
	bool m_globallyVisible;

	void updateGlobalVisibility();

public:
	explicit Path(geometry::Polyline&& basePolyline, const std::string& name, const PathSettings& settings);
	explicit Path(const Path& other);
	explicit Path();

	static ListUPtr FromPolylines(geometry::Polyline::List&& polylines, const PathSettings& settings,
								  const std::string& layerName);

	Layer& layer();
	const Layer& layer() const;
	void setLayer(Layer& layer);

	const geometry::Polyline& basePolyline() const;
	geometry::Polyline::List finalPolylines() const;

	model::OffsettedPath* offsettedPath() const;
	void offset(float margin, float minimumPolylineLength, float minimumArcLength);
	void resetOffset();
	void pocket(const Path::ListCPtr& islands, float scaledRadius, float minimumPolylineLength, float minimumArcLength);

	void transform(const QTransform& matrix);

	geometry::Rect boundingRect() const;

	bool isPoint() const;

	const PathSettings& settings() const;
	PathSettings& settings();

	geometry::CuttingDirection cuttingDirection() const;

	bool globallyVisible() const;

Q_SIGNALS:
	void globalVisibilityChanged(bool globallyVisible);
	void offsettedPathChanged();
	void basePolylineTransformed();
};

class Layer : public Renderable, public common::Aggregable<Layer>
{
	Q_OBJECT;

	friend serializer::Access<Layer>;

private:
	Path::ListUPtr m_children;

	void assignSelfToChildren();

public:
	explicit Layer(const std::string& name, Path::ListUPtr&& children);
	explicit Layer() = default;
	explicit Layer(const Layer& other);
	;

	int childrenCount() const;
	Path& childrenAt(int index);
	const Path& childrenAt(int index) const;
	int childIndexFor(const Path& child) const;

	template<class Functor>
	void forEachChild(Functor&& functor)
	{
		for (Path::UPtr& child : m_children) {
			functor(*child);
		}
	}
};

}

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

Path::Path(geometry::Polyline&& basePolyline, const std::string& name, const PathSettings& settings)
	: Renderable(name)
	, m_basePolyline(basePolyline)
	, m_settings(settings)
	, m_globallyVisible(true)
{
	connect(this, &Path::visibilityChanged, this, &Path::updateGlobalVisibility);
}

Path::Path(const Path& other)
	: Renderable(other)
	, m_basePolyline(other.m_basePolyline)
	, m_settings(other.m_settings)
	, m_globallyVisible(other.m_globallyVisible)
{
	connect(this, &Path::visibilityChanged, this, &Path::updateGlobalVisibility);

	if (other.m_offsettedPath) {
		m_offsettedPath = std::make_unique<OffsettedPath>(*other.m_offsettedPath);
	}
}

Path::Path()
{
	connect(this, &Path::visibilityChanged, this, &Path::updateGlobalVisibility);
}

Path::ListUPtr Path::FromPolylines(geometry::Polyline::List&& polylines, const PathSettings& settings,
								   const std::string& layerName)
{
	const int size = polylines.size();
	Path::ListUPtr paths(size);

	for (int i = 0; i < size; ++i) {
		static constexpr char pathNameFormat[] = "({}) {}";
		const std::string pathName = fmt::format(pathNameFormat, layerName, i);
		paths[i].reset(new Path(std::move(polylines[i]), pathName, settings));
	}

	return paths;
}

Layer& Path::layer()
{
	return *m_layer;
}

const Layer& Path::layer() const
{
	return *m_layer;
}

void Path::setLayer(Layer& layer)
{
	m_layer = &layer;
	updateGlobalVisibility();
	connect(m_layer, &Layer::visibilityChanged, this, &Path::updateGlobalVisibility);
}

const geometry::Polyline& Path::basePolyline() const
{
	return m_basePolyline;
}

geometry::Polyline::List Path::finalPolylines() const
{
	return m_offsettedPath ? m_offsettedPath->polylines() : geometry::Polyline::List { m_basePolyline };
}

model::OffsettedPath* Path::offsettedPath() const
{
	return m_offsettedPath.get();
}

void Path::offset(float margin, float minimumPolylineLength, float minimumArcLength)
{
	geometry::Polyline::List offsettedPolylines = m_basePolyline.offsetted(margin);
	geometry::filter::Cleaner cleaner(std::move(offsettedPolylines), minimumPolylineLength, minimumArcLength);

	const OffsettedPath::Direction direction
		= (margin > 0.0f) ? OffsettedPath::Direction::LEFT : OffsettedPath::Direction::RIGHT;

	geometry::Polyline::List cleanedPolylines = cleaner.polylines();
	if (!cleanedPolylines.empty()) {
		m_offsettedPath = std::make_unique<OffsettedPath>(std::move(cleanedPolylines), direction);
	}

	emit offsettedPathChanged();
}

void Path::resetOffset()
{
	m_offsettedPath.reset();

	emit offsettedPathChanged();
}

void Path::pocket(const Path::ListCPtr& islands, float scaledRadius, float minimumPolylineLength,
				  float minimumArcLength)
{
	geometry::Polyline::ListCPtr polylineIslands(islands.size());
	std::transform(islands.begin(), islands.end(), polylineIslands.begin(), [](const Path* path) {
		return &path->basePolyline();
	});

	geometry::Pocketer pocketer(m_basePolyline, polylineIslands, scaledRadius, minimumPolylineLength);
	geometry::filter::Cleaner cleaner(std::move(pocketer.polylines()), minimumPolylineLength, minimumArcLength);

	// Pocket is left when polyline is CCW winded.
	static const OffsettedPath::Direction basePolylineOrientationToPocketDirection[] = {
		OffsettedPath::Direction::RIGHT, // Orientation::CW
		OffsettedPath::Direction::LEFT // Orientation::CCW
	};
	const OffsettedPath::Direction direction
		= basePolylineOrientationToPocketDirection[static_cast<int>(pocketer.borderOrientation())];

	m_offsettedPath = std::make_unique<OffsettedPath>(cleaner.polylines(), direction);

	emit offsettedPathChanged();
}

void Path::transform(const QTransform& matrix)
{
	m_basePolyline.transform(matrix);
	emit basePolylineTransformed();

	if (m_offsettedPath) {
		m_offsettedPath->transform(matrix);
	}
}

geometry::Rect Path::boundingRect() const
{
	return (m_offsettedPath) ? m_offsettedPath->boundingRect() : m_basePolyline.boundingRect();
}

bool Path::isPoint() const
{
	return m_basePolyline.isPoint();
}

const model::PathSettings& Path::settings() const
{
	return m_settings;
}

model::PathSettings& Path::settings()
{
	return m_settings;
}

geometry::CuttingDirection Path::cuttingDirection() const
{
	return (m_offsettedPath) ? m_offsettedPath->cuttingDirection() : geometry::CuttingDirection::FORWARD;
}

bool Path::globallyVisible() const
{
	return m_globallyVisible;
}

}

#include "path.moc"
