module;

#include <QGraphicsPathItem>
#include <QStyleOptionGraphicsItem>
#include <QPen>
#include <QPainter>
#include <QtCore/qtmochelpers.h>

export module view.view2d.basicpathitem;

import model.path;

export namespace view::view2d
{

class BasicPathItem : public QObject, public QGraphicsPathItem
{
	Q_OBJECT;

private:
	model::Path& m_path;
	bool m_outsideSelectionBlocked;

public:
	explicit BasicPathItem(model::Path& path);

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

	const model::Path& path() const;
	virtual void setSelected(bool selected);

protected Q_SLOTS:
	QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
	void selectedChanged(bool selected);
	void visibilityChanged(bool visible);
	virtual void basePolylineTransformed() = 0;
};

}

namespace view::view2d
{

namespace
{

	const QBrush normalBrush(Qt::white);
	const QBrush selectBrush(QColor(80, 0, 255));
	const QPen normalPen(normalBrush, 0.0f);
	const QPen selectPen(selectBrush, 0.0f);

}

BasicPathItem::BasicPathItem(model::Path& path)
	: m_path(path)
	, m_outsideSelectionBlocked(false)
{
	setPen(normalPen);
	setFlag(ItemIsSelectable);
	setVisible(m_path.globallyVisible());

	connect(&m_path, &model::Path::selectedChanged, this, &BasicPathItem::selectedChanged);
	connect(&m_path, &model::Path::globalVisibilityChanged, this, &BasicPathItem::visibilityChanged);
	connect(&m_path, &model::Path::basePolylineTransformed, this, &BasicPathItem::basePolylineTransformed);
}

void BasicPathItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, [[maybe_unused]] QWidget* widget)
{
	QStyleOptionGraphicsItem fixedOption(*option);

	if (fixedOption.state & QStyle::State_Selected) {
		fixedOption.state &= ~QStyle::State_Selected;
		setPen(selectPen);
	} else {
		setPen(normalPen);
	}

	painter->setPen(pen());
}

const model::Path& BasicPathItem::path() const
{
	return m_path;
}

void BasicPathItem::setSelected(bool selected)
{
	QAbstractGraphicsShapeItem::setSelected(selected);
}

QVariant BasicPathItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
	if (change & ItemSelectedChange) {
		m_outsideSelectionBlocked = true;
		m_path.setSelected(isSelected());
		m_outsideSelectionBlocked = false;
	}

	return QAbstractGraphicsShapeItem::itemChange(change, value);
}

void BasicPathItem::selectedChanged(bool selected)
{
	if (!m_outsideSelectionBlocked) {
		setSelected(selected);
	}
}

void BasicPathItem::visibilityChanged(bool visible)
{
	setVisible(visible);
}

}

#include "basicpathitem.moc"
