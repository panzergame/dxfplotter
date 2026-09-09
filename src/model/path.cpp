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

class Path : public Renderable, public common::Aggregable<Path>
{
	Q_OBJECT;

	friend serializer::Access<Path>;

private:
	geometry::Polyline m_basePolyline;
	std::unique_ptr<model::OffsettedPath> m_offsettedPath;
	PathSettings m_settings;

public:
	explicit Path(geometry::Polyline&& basePolyline, const std::string& name, const PathSettings& settings)
		: Renderable(name)
		, m_basePolyline(basePolyline)
		, m_settings(settings)
	{
	}

	explicit Path(const Path& other)
		: Renderable(other)
		, m_basePolyline(other.m_basePolyline)
		, m_settings(other.m_settings)
	{
		if (other.m_offsettedPath) {
			m_offsettedPath = std::make_unique<OffsettedPath>(*other.m_offsettedPath);
		}
	}

	explicit Path() = default;

	static Path::ListUPtr FromPolylines(geometry::Polyline::List&& polylines, const PathSettings& settings,
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

	const geometry::Polyline& basePolyline() const { return m_basePolyline; }

	geometry::Polyline::List finalPolylines() const
	{
		return m_offsettedPath ? m_offsettedPath->polylines() : geometry::Polyline::List { m_basePolyline };
	}

	model::OffsettedPath* offsettedPath() const { return m_offsettedPath.get(); }

	void offset(float margin, float minimumPolylineLength, float minimumArcLength)
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

	void resetOffset()
	{
		m_offsettedPath.reset();

		emit offsettedPathChanged();
	}

	void pocket(const Path::ListCPtr& islands, float scaledRadius, float minimumPolylineLength, float minimumArcLength)
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

	void transform(const QTransform& matrix)
	{
		m_basePolyline.transform(matrix);
		emit basePolylineTransformed();

		if (m_offsettedPath) {
			m_offsettedPath->transform(matrix);
		}
	}

	geometry::Rect boundingRect() const
	{
		return (m_offsettedPath) ? m_offsettedPath->boundingRect() : m_basePolyline.boundingRect();
	}

	bool isPoint() const { return m_basePolyline.isPoint(); }

	const model::PathSettings& settings() const { return m_settings; }

	model::PathSettings& settings() { return m_settings; }

	geometry::CuttingDirection cuttingDirection() const
	{
		return (m_offsettedPath) ? m_offsettedPath->cuttingDirection() : geometry::CuttingDirection::FORWARD;
	}

Q_SIGNALS:
	void offsettedPathChanged();
	void basePolylineTransformed();
};

}

#include "path.moc"
