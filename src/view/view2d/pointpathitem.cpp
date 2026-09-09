module;

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QtCore/qtmochelpers.h>

export module view.view2d.pointpathitem;

import view.view2d.basicpathitem;
import view.view2d.offsettedpolylinepathitem;
import model.path;

export namespace view::view2d
{

/** @brief Graphics path item meant to display polylines with length.
 */
class PointPathItem : public BasicPathItem
{
	Q_OBJECT;

private:
	QPainterPath shapePath() const
	{
		QPainterPath path;
		constexpr float width = 0.05f; // TODO const or config
		path.addEllipse(0.0f, 0.0f, width, width);

		return path;
	}

	void setupPosition()
	{
		const QPointF point = path().basePolyline().start().toPointF();
		setPos(point);
	}

public:
	explicit PointPathItem(model::Path& path)
		: BasicPathItem(path)
	{
		setPath(shapePath());
		setFlag(QGraphicsItem::ItemIgnoresTransformations);

		setupPosition();
	}

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override
	{
		BasicPathItem::paint(painter, option, widget);

		painter->drawLine(-1.0f, 0.0f, 1.0f, 0.0f);
		painter->drawLine(0.0f, -1.0f, 0.0f, 1.0f);
	}

protected:
	void basePolylineTransformed() override { setupPosition(); }
};

}

#include "pointpathitem.moc"
