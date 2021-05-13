#pragma once

#include <model/path.h>

#include <QGraphicsPathItem>

namespace View::View2d
{

class OffsetedPolylinePathItem : public QObject, public QGraphicsPathItem
{
	Q_OBJECT;

private:
	const Model::Path &m_path;

	QPainterPath m_paintPath;

	QPainterPath paintPath() const;

	QPainterPath shape() const override;

public:
	explicit OffsetedPolylinePathItem(const Model::Path &path);

	void selected();
	void deselected();

protected Q_SLOTS:
	void pathChanged();
};

}
