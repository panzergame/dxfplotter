#include <pointpathitem.h>

#include <QPainter>
#include <QDebug>

namespace View::View2d
{

QPainterPath PointPathItem::shapePath() const
{
	QPainterPath path;
	constexpr float width = 0.05f; // TODO const or config
	path.addEllipse(m_point.toPointF(), width, width);

	return path;
}

PointPathItem::PointPathItem(Model::Path *path)
	:BasicPathItem(path),
	m_point(m_path->basePolyline().start()),
	m_shapePath(shapePath())
{
}

void PointPathItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	BasicPathItem::paint(painter, option, widget);

	painter->drawPoint(m_point.toPointF());
}

QPainterPath PointPathItem::shape() const
{
	return m_shapePath;
}

QRectF PointPathItem::boundingRect() const
{
	return m_shapePath.boundingRect();
}

}
