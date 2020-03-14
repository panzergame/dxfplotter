#pragma once

#include <model/path.h>

#include <QGraphicsPathItem>
#include <QPen>

namespace View
{

class PathItem : public QObject, public QGraphicsPathItem
{
	Q_OBJECT;

private:
	Model::Path *m_path;

	QPainterPath paintPath();

public:
	explicit PathItem(Model::Path *path);

protected Q_SLOTS:
	void selected();
	void deselected();
};

}
