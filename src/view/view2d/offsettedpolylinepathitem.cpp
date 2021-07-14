#include <offsettedpolylinepathitem.h>
#include <bulgepainter.h>

#include <geometry/arc.h>

#include <QStyleOptionGraphicsItem>
#include <QPen>
#include <QDebug>

namespace View::View2d
{

static const QBrush normalBrush(Qt::magenta);
static const QBrush selectBrush(Qt::red);
static const QPen normalPen(normalBrush, 0.0f);
static const QPen selectPen(selectBrush, 0.0f);

QPainterPath OffsettedPolylinePathItem::paintPath() const
{
	const Geometry::Polyline::List polylines = m_offsettedPath.offsettedPolylines();

	QPainterPath rootPainter;

	for (const Geometry::Polyline &polyline : polylines) {
		QPainterPath painter(polyline.start().toPointF());

		BulgePainter functor(painter);
		polyline.forEachBulge(functor);

		rootPainter.addPath(painter);
	}

	return rootPainter;
}

QPainterPath OffsettedPolylinePathItem::shape() const
{
	return QPainterPath();
}

OffsettedPolylinePathItem::OffsettedPolylinePathItem(const Model::OffsettedPath &offsettedPath)
	:QGraphicsPathItem(QPainterPath()),
	m_offsettedPath(offsettedPath),
	m_paintPath(paintPath())
{
	setPen(normalPen);
	setPath(m_paintPath);
}

void OffsettedPolylinePathItem::selected()
{
	setPen(selectPen);
}

void OffsettedPolylinePathItem::deselected()
{
	setPen(normalPen);
}

}
