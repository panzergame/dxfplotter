module;

#include <cstdint>
#include <QObject>
#include <QtCore/qtmochelpers.h>

export module model.toolpath;

import common.aggregable;
import geometry.polyline;
import geometry.rect;
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
	geometry::Polyline::List m_polylines;
	geometry::CuttingDirection m_cuttingDirection;

public:
	explicit ToolPath(geometry::Polyline::List&& offsettedPolylines, geometry::CuttingDirection cuttingDirection)
		: m_polylines(offsettedPolylines)
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

	const geometry::Polyline::List& polylines() const { return m_polylines; }

	geometry::CuttingDirection cuttingDirection() const { return m_cuttingDirection; }

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

#include "toolpath.moc"
