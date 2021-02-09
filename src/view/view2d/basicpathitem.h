#pragma once

#include <model/path.h>

#include <QAbstractGraphicsShapeItem>

namespace View::View2d
{

class BasicPathItem : public QObject, public QAbstractGraphicsShapeItem
{
	Q_OBJECT;

protected:
	Model::Path *m_path;
	bool m_outsideSelectionBlocked;

public:
	explicit BasicPathItem(Model::Path *path);

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	Model::Path *path() const;
	virtual void setSelected(bool selected);

protected Q_SLOTS:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
	void selectedChanged(bool selected);
	void visibilityChanged(bool visible);
};

}
