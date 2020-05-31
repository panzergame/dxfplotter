#pragma once

#include <model/path.h>

#include <view/offsetedpathitem.h>

#include <QGraphicsPathItem>

namespace View
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

public:
	explicit PathItem(Model::Path *path);

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	Model::Path *path() const;
	QPainterPath shape() const override;

protected Q_SLOTS:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
	void selected();
	void deselected();
};

}
