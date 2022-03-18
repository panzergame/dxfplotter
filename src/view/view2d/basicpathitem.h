#pragma once

#include <model/path.h>

#include <QAbstractGraphicsShapeItem>

namespace view::view2d
{

class BasicPathItem : public QObject, public QAbstractGraphicsShapeItem
{
	Q_OBJECT;

private:
	model::Path &m_path;
	bool m_outsideSelectionBlocked;

public:
	explicit BasicPathItem(model::Path &path);

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	const model::Path &path() const;
	virtual void setSelected(bool selected);

protected Q_SLOTS:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
	void selectedChanged(bool selected);
	void visibilityChanged(bool visible);
	virtual void basePolylineTransformed() = 0;
};

}
