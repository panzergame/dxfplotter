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

public:
	explicit PathItem(Model::Path *path);
};

}
