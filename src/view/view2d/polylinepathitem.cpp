module;

#include <QGraphicsPathItem>
#include <QStyleOptionGraphicsItem>
#include <QPainterPath>
#include <QPen>

#include <QPainter>
#include <QDebug>
#include <QtCore/qtmochelpers.h>

export module view.view2d.polylinepathitem;

import view.view2d.basicpathitem;
import view.view2d.bulgepainter;
import view.view2d.toolpathitem;
import geometry.polyline;
import model.toolpath;
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

	std::unique_ptr<ToolPathItem> m_toolPath;

	QPainterPath paintPath() const
	{
		const geometry::Polyline& polyline = path().basePolyline();

		QPainterPath painter(polyline.start().toPointF());

		BulgePainter functor(painter);
		polyline.forEachBulge(functor);

		return painter;
	}

	static QPainterPath shapePath(const QPainterPath& basePath)
	{
		QPainterPathStroker stroker;
		stroker.setWidth(0.05f); // TODO const or config
		stroker.setCapStyle(Qt::RoundCap);
		stroker.setJoinStyle(Qt::RoundJoin);

		return stroker.createStroke(basePath);
	}

	void setupPaths()
	{
		m_paintPath = paintPath();
		setPath(m_paintPath);
		m_shapePath = shapePath(m_paintPath);
	}

	void updateToolPath()
	{
		model::ToolPath* toolPath = path().toolPath();
		if (toolPath) {
			m_toolPath = std::make_unique<ToolPathItem>(*toolPath);
			// Link our offsetted path item for drawing
			m_toolPath->setParentItem(this); // TODO use QGraphicsItemGroup
		} else {
			m_toolPath.reset();
		}
	}

	// Change selected state and propagate to sub paths (e.g offsetted path).
	void setSelected(bool selected) override
	{
		BasicPathItem::setSelected(selected);

		if (m_toolPath) {
			m_toolPath->setSelected(selected);
		}
	}

public:
	explicit PolylinePathItem(model::Path& path)
		: BasicPathItem(path)
	{
		setupPaths();

		updateToolPath();

		connect(&path, &model::Path::toolPathChanged, this, &PolylinePathItem::updateToolPath);
	}

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override
	{
		BasicPathItem::paint(painter, option, widget);

		painter->drawPath(m_paintPath);
	}

	QPainterPath shape() const override { return m_shapePath; }

protected:
	void basePolylineTransformed() override { setupPaths(); }
};

}

#include "polylinepathitem.moc"
