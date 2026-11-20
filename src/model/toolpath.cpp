module;

#include <cstdint>
#include <utility>
#include <QObject>
#include <QtCore/qtmochelpers.h>

export module model.toolpath;

import common.aggregable;
import geometry.polyline;
import geometry.rect;
import geometry.tooloperation.tooloperation;
import geometry.utils;
import model.pathsettings;
import model.renderable;
import serializer.access;

export namespace model
{

class ToolPath : public QObject
{
	Q_OBJECT

	friend serializer::Access<ToolPath>;

private:
	geometry::tooloperation::ToolPolyline::List m_polylines;
	geometry::CuttingDirection m_cuttingDirection;

public:
	explicit ToolPath(geometry::tooloperation::ToolPolyline::List&& polylines,
					  geometry::CuttingDirection cuttingDirection)
		: m_polylines(std::move(polylines))
		, m_cuttingDirection(cuttingDirection)
	{
	}

	explicit ToolPath(const ToolPath& other)
		: QObject()
		, m_polylines(other.m_polylines)
		, m_cuttingDirection(other.m_cuttingDirection)
	{
	}

	explicit ToolPath() = default;

	const geometry::tooloperation::ToolPolyline::List& polylines() const { return m_polylines; }

	geometry::CuttingDirection cuttingDirection() const { return m_cuttingDirection; }

	void transform(const QTransform& matrix)
	{
		for (geometry::tooloperation::ToolPolyline& toolPolyline : m_polylines) {
			toolPolyline.polyline.transform(matrix);
			if (toolPolyline.entry) {
				toolPolyline.entry->transform(matrix);
			}
		}

		emit polylinesTransformed();
	}

	geometry::Rect boundingRect() const
	{
		const auto toolPolylineBoundingRect = [](const geometry::tooloperation::ToolPolyline& toolPolyline) {
			const geometry::Rect polylineBoundingRect = toolPolyline.polyline.boundingRect();
			return toolPolyline.entry ? (polylineBoundingRect | toolPolyline.entry->boundingRect())
									  : polylineBoundingRect;
		};

		geometry::tooloperation::ToolPolyline::List::const_iterator it = m_polylines.begin();
		const geometry::Rect firstBoundingRest = toolPolylineBoundingRect(*it++);

		return std::transform_reduce(it, m_polylines.end(), firstBoundingRest, std::bit_or(),
									 toolPolylineBoundingRect);
	}

Q_SIGNALS:
	void polylinesTransformed();
};

}

#include "toolpath.moc"
