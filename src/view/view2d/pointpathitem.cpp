#include <pointpathitem.h>

#include <QPainter>
#include <QStyleOptionGraphicsItem>

namespace view::view2d
{

QPainterPath PointPathItem::shapePath() const
{
	QPainterPath path;
	constexpr float width = 0.05f; // TODO const or config
	path.addEllipse(0.0f, 0.0f, width, width);

	return path;
}

void PointPathItem::setupPosition()
{
	m_point = m_path.basePolyline().start().toPointF();
	setPos(m_point);
}

PointPathItem::PointPathItem(model::Path& path)
	:BasicPathItem(path),
	m_shapePath(shapePath())
{
	setFlag(QGraphicsItem::ItemIgnoresTransformations);

	setupPosition();
}

void PointPathItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	BasicPathItem::paint(painter, option, widget);

	painter->drawLine(-1.0f, 0.0f, 1.0f, 0.0f);
	painter->drawLine(0.0f, -1.0f, 0.0f, 1.0f);
}

QPainterPath PointPathItem::shape() const
{
	return m_shapePath;
}

QRectF PointPathItem::boundingRect() const
{
	return m_shapePath.boundingRect();
}

void PointPathItem::basePolylineTransformed()
{
	setupPosition();
}

}
