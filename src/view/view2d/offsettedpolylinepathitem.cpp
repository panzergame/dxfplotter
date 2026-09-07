module;

#include <QGraphicsPathItem>
#include <QStyleOptionGraphicsItem>
#include <QPen>
#include <QDebug>
#include <QtCore/qtmochelpers.h>

export module view.view2d.offsettedpolylinepathitem;
import view.view2d.bulgepainter;

import geometry.arc;
import model.path;
import geometry.polyline;
import model.offsettedpath;

export namespace view::view2d
{

class OffsettedPolylinePathItem : public QObject, public QGraphicsPathItem
{
	Q_OBJECT;

private:
	const model::OffsettedPath& m_offsettedPath;
	QPainterPath m_paintPath;

	QPainterPath paintPath() const;

	QPainterPath shape() const override;

	void setupPaths();

public:
	explicit OffsettedPolylinePathItem(const model::OffsettedPath& offsettedPath);

	void selected();
	void deselected();

protected Q_SLOTS:
	void polylinesTransformed();
};

}

namespace view::view2d
{

namespace
{

	const QBrush normalBrush(Qt::magenta);
	const QBrush selectBrush(Qt::red);
	const QPen normalPen(normalBrush, 0.0f);
	const QPen selectPen(selectBrush, 0.0f);

}

QPainterPath OffsettedPolylinePathItem::paintPath() const
{
	const geometry::Polyline::List polylines = m_offsettedPath.polylines();

	QPainterPath rootPainter;

	for (const geometry::Polyline& polyline : polylines) {
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

OffsettedPolylinePathItem::OffsettedPolylinePathItem(const model::OffsettedPath& offsettedPath)
	: QGraphicsPathItem(QPainterPath())
	, m_offsettedPath(offsettedPath)
{
	setupPaths();
	setPen(normalPen);

	connect(&offsettedPath, &model::OffsettedPath::polylinesTransformed, this,
			&OffsettedPolylinePathItem::polylinesTransformed);
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

#include "offsettedpolylinepathitem.moc"
