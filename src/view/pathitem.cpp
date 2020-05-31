#include <pathitem.h>
#include <bulgepainter.h>

#include <geometry/arc.h>

#include <QtMath>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QDebug>

namespace View
{

static const QBrush normalBrush(Qt::white);
static const QBrush selectBrush(QColor(80, 0, 255));
static const QPen normalPen(normalBrush, 0.0f);
static const QPen selectPen(selectBrush, 0.0f);

QPainterPath PathItem::paintPath() const
{
	const Geometry::Polyline &polyline = m_path->basePolyline();

	QPainterPath painter(polyline.start().toPointF());

	BulgePainter functor(painter);
	polyline.forEachBulge(functor);

	return painter;
}

QPainterPath PathItem::shapePath() const
{
	QPainterPathStroker stroker;
	stroker.setWidth(0.05f); // TODO const or config
	stroker.setCapStyle(Qt::RoundCap);
	stroker.setJoinStyle(Qt::RoundJoin);

	return stroker.createStroke(m_paintPath);
}

PathItem::PathItem(Model::Path *path)
	:QGraphicsPathItem(QPainterPath()),
	m_path(path),
	m_outsideSelectionBlocked(false),
	m_paintPath(paintPath()),
	m_shapePath(shapePath()),
	m_offsetedPath(path)
{
	setPen(normalPen);
	setPath(m_paintPath);
	setFlag(ItemIsSelectable);

	// Link our offsetted path item for drawing
	m_offsetedPath.setParentItem(this); // TODO use QGraphicsItemGroup

	connect(m_path, &Model::Path::selected, this, &PathItem::selected);
	connect(m_path, &Model::Path::deselected, this, &PathItem::deselected);
}

void PathItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QStyleOptionGraphicsItem fixedOption(*option);

	if (fixedOption.state & QStyle::State_Selected) {
		fixedOption.state &= ~QStyle::State_Selected;
		setPen(selectPen);
	}
	else {
		setPen(normalPen);
	}

    QGraphicsPathItem::paint(painter, &fixedOption, widget);
}

Model::Path *PathItem::path() const
{
	return m_path;
}

QPainterPath PathItem::shape() const
{
	return m_shapePath;
}

QVariant PathItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change & ItemSelectedChange) {
		m_outsideSelectionBlocked = true;
		if (isSelected()) {
			m_path->select();
		}
		else {
			m_path->deselect();
		}
		m_outsideSelectionBlocked = false;
	}

	return QGraphicsItem::itemChange(change, value);
}

void PathItem::selected()
{
	if (!m_outsideSelectionBlocked) {
		setSelected(true);
	}

	m_offsetedPath.selected();
}

void PathItem::deselected()
{
	if (!m_outsideSelectionBlocked) {
		setSelected(false);
	}

	m_offsetedPath.deselected();
}


}
