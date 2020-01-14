#pragma once

#include <model/path.h>

#include <QGraphicsPathItem>
#include <QPen>

namespace View
{

class PathItem : public QGraphicsPathItem
{
private:
	Model::Path *m_path;

	QPainterPath paintPath();

protected Q_SLOTS:
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

public:
	explicit PathItem(Model::Path *path);

	QPainterPath shape() const override;
};

}
