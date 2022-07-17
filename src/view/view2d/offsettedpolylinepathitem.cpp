#include <offsettedpolylinepathitem.h>
#include <bulgepainter.h>

#include <geometry/arc.h>

#include <QStyleOptionGraphicsItem>
#include <QPen>
#include <QDebug>

namespace view::view2d
{

static const QBrush normalBrush(Qt::magenta);
static const QBrush selectBrush(Qt::red);
static const QPen normalPen(normalBrush, 0.0f);
static const QPen selectPen(selectBrush, 0.0f);

QPainterPath OffsettedPolylinePathItem::paintPath() const
{
	const geometry::Polyline::List polylines = m_offsettedPath.polylines();

	QPainterPath rootPainter;

	for (const geometry::Polyline &polyline : polylines) {
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

void OffsettedPolylinePathItem::setupPaths()
{
	m_paintPath = paintPath();
	setPath(m_paintPath);
}

OffsettedPolylinePathItem::OffsettedPolylinePathItem(const model::OffsettedPath &offsettedPath)
	:QGraphicsPathItem(QPainterPath()),
	m_offsettedPath(offsettedPath)
{
	setupPaths();
	setPen(normalPen);

	connect(&offsettedPath, &model::OffsettedPath::polylinesTransformed, this, &OffsettedPolylinePathItem::polylinesTransformed);
}

void OffsettedPolylinePathItem::selected()
{
	setPen(selectPen);
}

void OffsettedPolylinePathItem::deselected()
{
	setPen(normalPen);
}

void OffsettedPolylinePathItem::polylinesTransformed()
{
	setupPaths();

	update();
}

}
