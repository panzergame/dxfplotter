#include <basicpathitem.h>

#include <QStyleOptionGraphicsItem>
#include <QPen>
#include <QPainter>

namespace View::View2d
{

static const QBrush normalBrush(Qt::white);
static const QBrush selectBrush(QColor(80, 0, 255));
static const QPen normalPen(normalBrush, 0.0f);
static const QPen selectPen(selectBrush, 0.0f);

BasicPathItem::BasicPathItem(Model::Path &path)
	:m_path(path),
	m_outsideSelectionBlocked(false)
{
	setPen(normalPen);
	setFlag(ItemIsSelectable);

	connect(&m_path, &Model::Path::selectedChanged, this, &BasicPathItem::selectedChanged);
	connect(&m_path, &Model::Path::globalVisibilityChanged, this, &BasicPathItem::visibilityChanged);
}

void BasicPathItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QStyleOptionGraphicsItem fixedOption(*option);

	if (fixedOption.state & QStyle::State_Selected) {
		fixedOption.state &= ~QStyle::State_Selected;
		setPen(selectPen);
	}
	else {
		setPen(normalPen);
	}

	painter->setPen(pen());
}

const Model::Path& BasicPathItem::path() const
{
	return m_path;
}

void BasicPathItem::setSelected(bool selected)
{
	QAbstractGraphicsShapeItem::setSelected(selected);
}

QVariant BasicPathItem::itemChange(GraphicsItemChange change, const QVariant &value)
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
