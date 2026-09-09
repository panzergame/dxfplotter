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

namespace view::view2d
{

namespace
{

	const QBrush normalBrush(Qt::magenta);
	const QBrush selectBrush(Qt::red);
	const QPen normalPen(normalBrush, 0.0f);
	const QPen selectPen(selectBrush, 0.0f);

}

}

export namespace view::view2d
{

class OffsettedPolylinePathItem : public QObject, public QGraphicsPathItem
{
	Q_OBJECT;

private:
	const model::OffsettedPath& m_offsettedPath;
	QPainterPath m_paintPath;

	QPainterPath paintPath() const
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

	QPainterPath shape() const override { return QPainterPath(); }

	void setupPaths()
	{
		m_paintPath = paintPath();
		setPath(m_paintPath);
	}

public:
	explicit OffsettedPolylinePathItem(const model::OffsettedPath& offsettedPath)
		: QGraphicsPathItem(QPainterPath())
		, m_offsettedPath(offsettedPath)
	{
		setupPaths();
		setPen(normalPen);

		connect(&offsettedPath, &model::OffsettedPath::polylinesTransformed, this,
				&OffsettedPolylinePathItem::polylinesTransformed);
	}

	void selected() { setPen(selectPen); }

	void deselected() { setPen(normalPen); }

protected Q_SLOTS:
	void polylinesTransformed()
	{
		setupPaths();

		update();
	}
};

}

#include "offsettedpolylinepathitem.moc"
