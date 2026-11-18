module;

#include <cmath>
#include <cstdint>
#include <fmt/format.h>
#include <optional>
#include <QVector2D>
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
import serializer.access;

namespace model
{

/** Shift every bulge forward by the blade offset along its tangent so the trailing blade tip follows
 * the polyline, and pivot the holder around each vertex where the tangent breaks.
 */
geometry::Polyline DragKnifeCompensated(const geometry::Polyline& polyline, float bladeOffset,
										float minimumSwivelAngle)
{
	geometry::Bulge::List bulges;
	std::optional<QVector2D> previousTangent;
	std::optional<QVector2D> firstTangent;

	// Below the minimum swivel angle the gap is bladeOffset * angle, closed by moving the next bulge start.
	const auto joint = [&](const QVector2D& vertex, const QVector2D& fromTangent, const QVector2D& toTangent) {
		const float turn = geometry::SignedAngle(fromTangent, toTangent);
		if (std::abs(turn) > minimumSwivelAngle) {
			bulges.emplace_back(vertex + fromTangent * bladeOffset, vertex + toTangent * bladeOffset,
								std::tan(turn / 4.0f));
		}
	};

	polyline.forEachBulge([&](const geometry::Bulge& bulge) {
		// Zero length bulges have no tangent.
		if (bulge.start() == bulge.end()) {
			return;
		}

		const QVector2D startTangent = bulge.startTangent();
		const QVector2D endTangent = bulge.endTangent();

		if (previousTangent) {
			joint(bulge.start(), *previousTangent, startTangent);
		} else {
			firstTangent = startTangent;
		}

		// Arc keeps its center and sweep angle, so its bulge tangent is unchanged.
		geometry::Bulge shifted(bulge.start() + startTangent * bladeOffset, bulge.end() + endTangent * bladeOffset,
								bulge.tangent());
		if (!bulges.empty()) {
			shifted.start() = bulges.back().end();
		}
		bulges.push_back(shifted);

		previousTangent = endTangent;
	});

	if (bulges.empty()) {
		return polyline;
	}

	if (polyline.isClosed()) {
		joint(polyline.start(), *previousTangent, *firstTangent);
		bulges.front().start() = bulges.back().end();
	}

	return geometry::Polyline(std::move(bulges));
}

}

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

	/** Holder path of a drag knife whose blade tip trails by bladeOffset, for final polylines cut in direction.
	 * Returned polylines are already in cutting direction.
	 */
	geometry::Polyline::List dragKnifeCompensated(float bladeOffset, float minimumSwivelAngle,
												  geometry::CuttingDirection direction) const
	{
		geometry::Polyline::List polylines = finalPolylines();
		for (geometry::Polyline& polyline : polylines) {
			if (direction == geometry::CuttingDirection::BACKWARD) {
				polyline.invert();
			}
			polyline = DragKnifeCompensated(polyline, bladeOffset, minimumSwivelAngle);
		}

		return polylines;
	}

Q_SIGNALS:
	void offsettedPathChanged();
	void basePolylineTransformed();
};

}

#include "path.moc"
