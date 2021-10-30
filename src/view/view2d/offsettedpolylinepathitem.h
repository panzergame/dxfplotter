#pragma once

#include <model/path.h>

#include <QGraphicsPathItem>

namespace view::view2d
{

class OffsettedPolylinePathItem : public QObject, public QGraphicsPathItem
{
	Q_OBJECT;

private:
	const model::OffsettedPath &m_offsettedPath;
	QPainterPath m_paintPath;

	QPainterPath paintPath() const;

	QPainterPath shape() const override;

	void setupPaths();

public:
	explicit OffsettedPolylinePathItem(const model::OffsettedPath &offsettedPath);

	void selected();
	void deselected();

protected Q_SLOTS:
	void polylinesTransformed();
};

}
