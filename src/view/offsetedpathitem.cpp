#include <offsetedpathitem.h>
#include <bulgepainter.h>

#include <geometry/arc.h>

#include <QStyleOptionGraphicsItem>
#include <QDebug>

namespace View
{

static const QBrush normalBrush(Qt::magenta);
static const QBrush selectBrush(Qt::red);
static const QPen normalPen(normalBrush, 0.0f);
static const QPen selectPen(selectBrush, 0.0f);

QPainterPath OffsetedPathItem::paintPath() const
{
	const Geometry::Polyline::List polylines = m_path->offsetedPolylines();

	QPainterPath rootPainter;

	for (const Geometry::Polyline &polyline : polylines) {
		QPainterPath painter(polyline.start().toPointF());

		BulgePainter functor(painter);
		polyline.forEachBulge(functor);

		rootPainter.addPath(painter);
	}

	return rootPainter;
}

QPainterPath OffsetedPathItem::shape() const
{
	return QPainterPath();
}

OffsetedPathItem::OffsetedPathItem(Model::Path *path)
	:QGraphicsPathItem(QPainterPath()),
	m_path(path),
	m_paintPath(paintPath())
{
	setPen(normalPen);
	setPath(m_paintPath);
}

void OffsetedPathItem::selected()
{
	setPen(selectPen);
}

void OffsetedPathItem::deselected()
{
	setPen(normalPen);
}


}
