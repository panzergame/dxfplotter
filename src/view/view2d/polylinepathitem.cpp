#include <polylinepathitem.h>
#include <bulgepainter.h>

#include <QPainter>

#include <QDebug> // TODO

namespace view::view2d
{

QPainterPath PolylinePathItem::paintPath() const
{
	const geometry::Polyline &polyline = m_path.basePolyline();

	QPainterPath painter(polyline.start().toPointF());

	BulgePainter functor(painter);
	polyline.forEachBulge(functor);

	return painter;
}

QPainterPath PolylinePathItem::shapePath(const QPainterPath& basePath)
{
	QPainterPathStroker stroker;
	stroker.setWidth(0.05f); // TODO const or config
	stroker.setCapStyle(Qt::RoundCap);
	stroker.setJoinStyle(Qt::RoundJoin);

	return stroker.createStroke(basePath);
}

void PolylinePathItem::setupPaths()
{
	m_paintPath = paintPath();
	m_shapePath = shapePath(m_paintPath);
}

void PolylinePathItem::updateOffsetedPath()
{
	model::OffsettedPath *offsettedPath = m_path.offsettedPath();
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

PolylinePathItem::PolylinePathItem(model::Path &path)
	:BasicPathItem(path)
{
	setupPaths();

	updateOffsetedPath();

	connect(&path, &model::Path::offsettedPathChanged, this, &PolylinePathItem::updateOffsetedPath);
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

void PolylinePathItem::basePolylineTransformed()
{
	setupPaths();
}

}
