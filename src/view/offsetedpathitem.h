#pragma once

#include <model/path.h>

#include <QGraphicsPathItem>

namespace View
{

class OffsetedPathItem : public QGraphicsPathItem
{
private:
	Model::Path *m_path;

	QPainterPath m_paintPath;

	QPainterPath paintPath() const;

	QPainterPath shape() const override;

public:
	explicit OffsetedPathItem(Model::Path *path);

	void selected();
	void deselected();
};

}
