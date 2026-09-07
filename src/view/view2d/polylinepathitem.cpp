module;

#include <QStyleOptionGraphicsItem>
#include <QPainterPath>

#include <QPainter>
#include <QDebug>
#include <QtCore/qtmochelpers.h>

export module view.view2d.polylinepathitem;
import view.view2d.bulgepainter;

import view.view2d.basicpathitem;
import view.view2d.offsettedpolylinepathitem;
import geometry.polyline;
import model.offsettedpath;
import model.path;

export namespace view::view2d
{

/** @brief Graphics path item meant to display polylines with length.
 */
class PolylinePathItem : public BasicPathItem
{
	Q_OBJECT;

private:
	QPainterPath m_paintPath;
	QPainterPath m_shapePath;

	// Item of offsetted polylines of the same path.
	std::unique_ptr<OffsettedPolylinePathItem> m_offsettedPath;

	QPainterPath paintPath() const;
	static QPainterPath shapePath(const QPainterPath& basePath);

	void setupPaths();

	void updateOffsetedPath();

	// Change selected state and propagate to sub paths (e.g offsetted path).
	void setSelected(bool selected) override;

public:
	explicit PolylinePathItem(model::Path& path);

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

	QPainterPath shape() const override;

protected:
	void basePolylineTransformed() override;
};

}

namespace view::view2d
{

QPainterPath PolylinePathItem::paintPath() const
{
	const geometry::Polyline& polyline = path().basePolyline();

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
	setPath(m_paintPath);
	m_shapePath = shapePath(m_paintPath);
}

void PolylinePathItem::updateOffsetedPath()
{
	model::OffsettedPath* offsettedPath = path().offsettedPath();
	if (offsettedPath) {
		m_offsettedPath = std::make_unique<OffsettedPolylinePathItem>(*offsettedPath);
		// Link our offsetted path item for drawing
		m_offsettedPath->setParentItem(this); // TODO use QGraphicsItemGroup
	} else {
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

PolylinePathItem::PolylinePathItem(model::Path& path)
	: BasicPathItem(path)
{
	setupPaths();

	updateOffsetedPath();

	connect(&path, &model::Path::offsettedPathChanged, this, &PolylinePathItem::updateOffsetedPath);
}

void PolylinePathItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	BasicPathItem::paint(painter, option, widget);

	painter->drawPath(m_paintPath);
}

QPainterPath PolylinePathItem::shape() const
{
	return m_shapePath;
}

void PolylinePathItem::basePolylineTransformed()
{
	setupPaths();
}

}

#include "polylinepathitem.moc"
