module;

#include <cstdint>
#include <serializer/access.h>
#include <QObject>
#include <QtCore/qtmochelpers.h>

export module model.offsettedpath;

import common.aggregable;
import geometry.polyline;
import geometry.rect;
import geometry.utils;
import model.pathsettings;
import model.renderable;
export namespace model
{

class OffsettedPath : public QObject
{
	Q_OBJECT

	friend serializer::Access<OffsettedPath>;

public:
	enum class Direction { LEFT = 0, RIGHT };

private:
	geometry::Polyline::List m_polylines;
	Direction m_direction;

public:
	explicit OffsettedPath(geometry::Polyline::List&& offsettedPolylines, Direction direction)
		: m_polylines(offsettedPolylines)
		, m_direction(direction)
	{
	}

	explicit OffsettedPath(const OffsettedPath& other)
		: QObject()
		, m_polylines(other.m_polylines)
		, m_direction(other.m_direction)
	{
	}

	explicit OffsettedPath() = default;

	const geometry::Polyline::List& polylines() const { return m_polylines; }

	geometry::CuttingDirection cuttingDirection() const
	{
		static const geometry::CuttingDirection offsetDirectionToCuttingDirection[] = {
			geometry::CuttingDirection::FORWARD, // OffsettedPath::Direction::LEFT
			geometry::CuttingDirection::BACKWARD // OffsettedPath::Direction::RIGHT
		};

		return offsetDirectionToCuttingDirection[static_cast<int>(m_direction)];
	}

	void transform(const QTransform& matrix)
	{
		for (geometry::Polyline& polyline : m_polylines) {
			polyline.transform(matrix);
		}

		emit polylinesTransformed();
	}

	geometry::Rect boundingRect() const
	{
		geometry::Polyline::List::const_iterator it = m_polylines.begin();
		const geometry::Rect firstBoundingRest = (it++)->boundingRect();

		return std::transform_reduce(it, m_polylines.end(), firstBoundingRest, std::bit_or(),
									 [](const geometry::Polyline& polyline) {
										 return polyline.boundingRect();
									 });
	}

Q_SIGNALS:
	void polylinesTransformed();
};

}

#include "offsettedpath.moc"
