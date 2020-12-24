#pragma once

#include <model/path.h>

#include <QGraphicsPathItem>

namespace View::View2d
{

class OffsetedPathItem : public QObject, public QGraphicsPathItem
{
	Q_OBJECT;

private:
	Model::Path *m_path;

	QPainterPath m_paintPath;

	QPainterPath paintPath() const;

	QPainterPath shape() const override;

public:
	explicit OffsetedPathItem(Model::Path *path);

	void selected();
	void deselected();

protected Q_SLOTS:
	void pathChanged();
};

}
