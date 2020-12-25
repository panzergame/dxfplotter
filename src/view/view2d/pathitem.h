#pragma once

#include <model/path.h>

#include <view/view2d/offsetedpathitem.h>

#include <QGraphicsPathItem>

namespace View::View2d
{

class PathItem : public QObject, public QGraphicsPathItem
{
	Q_OBJECT;

private:
	Model::Path *m_path;
	bool m_outsideSelectionBlocked;

	QPainterPath m_paintPath;
	QPainterPath m_shapePath;

	// Item of offseted polylines of the same path.
	OffsetedPathItem m_offsetedPath;

	QPainterPath paintPath() const;
	QPainterPath shapePath() const;

	// Change selected state and propagate to sub paths (e.g offseted path).
	void setSelected(bool selected);

public:
	explicit PathItem(Model::Path *path);

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	Model::Path *path() const;
	QPainterPath shape() const override;

protected Q_SLOTS:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
	void selectedChanged(bool selected);
};

}
