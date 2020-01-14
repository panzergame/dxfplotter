#include <pathitem.h>

namespace View
{

static const QBrush normalBrush(QColor(0, 0, 255));
static const QBrush selectBrush(QColor(255, 0, 255));
static const QPen normalPen(normalBrush, 0);
static const QPen selectPen(selectBrush, 0);

class PaintBulge
{
private:
	QPainterPath &m_painter;

public:
	explicit PaintBulge(QPainterPath &painter)
		:m_painter(painter)
	{
	}

	void operator()(const Model::Bulge &bulge)
	{
		m_painter.lineTo(bulge.end().toPointF());
	}
};

QPainterPath PathItem::paintPath()
{
	const Model::Polyline &polyline = m_path->polyline();

	QPainterPath painter(polyline.start().toPointF());

	PaintBulge functor(painter);
	polyline.travelAlong(functor);

	return painter;
}

void PathItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	setPen(selectPen);
	QGraphicsPathItem::hoverEnterEvent(event);
}

void PathItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	setPen(normalPen);
	QGraphicsPathItem::hoverLeaveEvent(event);
}

PathItem::PathItem(Model::Path *path)
	:QGraphicsPathItem(QPainterPath()),
	m_path(path)
{
	setAcceptHoverEvents(true);
	setPath(paintPath());

	setPen(normalPen);
}

QPainterPath PathItem::shape() const
{
	return path();
}

}
