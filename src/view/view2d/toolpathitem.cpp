module;

#include <QGraphicsPathItem>
#include <QStyleOptionGraphicsItem>
#include <QPen>
#include <QDebug>
#include <QtCore/qtmochelpers.h>

export module view.view2d.toolpathitem;
import view.view2d.bulgepainter;

import geometry.arc;
import model.path;
import geometry.polyline;
import geometry.tooloperation.tooloperation;
import model.toolpath;

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

class ToolPathItem : public QObject, public QGraphicsPathItem
{
	Q_OBJECT;

private:
	const model::ToolPath& m_toolPath;
	QPainterPath m_paintPath;

	QPainterPath paintPath() const
	{
		QPainterPath rootPainter;

		const auto addPolyline = [&rootPainter](const geometry::Polyline& polyline) {
			QPainterPath painter(polyline.start().toPointF());

			BulgePainter functor(painter);
			polyline.forEachBulge(functor);

			rootPainter.addPath(painter);
		};

		for (const geometry::tooloperation::ToolPolyline& toolPolyline : m_toolPath.polylines()) {
			if (toolPolyline.entry) {
				addPolyline(*toolPolyline.entry);
			}
			addPolyline(toolPolyline.polyline);
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
	explicit ToolPathItem(const model::ToolPath& toolPath)
		: QGraphicsPathItem(QPainterPath())
		, m_toolPath(toolPath)
	{
		setupPaths();
		setPen(normalPen);

		connect(&toolPath, &model::ToolPath::polylinesTransformed, this, &ToolPathItem::polylinesTransformed);
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

#include "toolpathitem.moc"
