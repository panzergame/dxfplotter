#include <polylinepathitem.h>
#include <bulgepainter.h>

#include <QPainter>

namespace View::View2d
{

QPainterPath PolylinePathItem::paintPath() const
{
	const Geometry::Polyline &polyline = m_path.basePolyline();

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

void PolylinePathItem::updateOffsetedPath()
{
	Model::OffsettedPath *offsettedPath = m_path.offsettedPath();
	if (offsettedPath) {
		m_offsettedPath = std::make_unique<OffsettedPolylinePathItem>(*offsettedPath);
		// Link our offsetted path item for drawing
		m_offsettedPath->setParentItem(this); // TODO use QGraphicsItemGroup
	}
	else {
		m_offsettedPath.reset();
	}
}

void PolylinePathItem::setSelected(bool selected)
{
	BasicPathItem::setSelected(selected);

	if (m_offsettedPath) {
		m_offsettedPath->setSelected(selected);
	}
}

PolylinePathItem::PolylinePathItem(Model::Path &path)
	:BasicPathItem(path),
	m_paintPath(paintPath()),
	m_shapePath(shapePath())
{
	connect(&path, &Model::Path::offsettedPathChanged, this, &PolylinePathItem::updateOffsetedPath);
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
