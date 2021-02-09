#include <polylinepathitem.h>
#include <bulgepainter.h>

#include <QPainter>

namespace View::View2d
{

QPainterPath PolylinePathItem::paintPath() const
{
	const Geometry::Polyline &polyline = m_path->basePolyline();

	QPainterPath painter(polyline.start().toPointF());

	BulgePainter functor(painter);
	polyline.forEachBulge(functor);

	return painter;
}

QPainterPath PolylinePathItem::shapePath() const
{
	QPainterPathStroker stroker;
	stroker.setWidth(0.05f); // TODO const or config
	stroker.setCapStyle(Qt::RoundCap);
	stroker.setJoinStyle(Qt::RoundJoin);

	return stroker.createStroke(m_paintPath);
}

void PolylinePathItem::setSelected(bool selected)
{
	BasicPathItem::setSelected(selected);

	m_offsetedPath.setSelected(selected);
}

PolylinePathItem::PolylinePathItem(Model::Path *path)
	:BasicPathItem(path),
	m_paintPath(paintPath()),
	m_shapePath(shapePath()),
	m_offsetedPath(path)
{
	// Link our offsetted path item for drawing
	m_offsetedPath.setParentItem(this); // TODO use QGraphicsItemGroup
}

void PolylinePathItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    BasicPathItem::paint(painter, option, widget);

	painter->drawPath(m_paintPath);
}

QPainterPath PolylinePathItem::shape() const
{
	return m_shapePath;
}

QRectF PolylinePathItem::boundingRect() const
{
	return m_shapePath.boundingRect();
}

}
