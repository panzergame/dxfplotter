#include <pathitem.h>
#include <bulgepainter.h>

#include <geometry/arc.h>

#include <QtMath>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QDebug>

namespace View::View2d
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

void PathItem::setSelected(bool selected)
{
	QGraphicsItem::setSelected(selected);

	m_offsetedPath.setSelected(selected);
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

	connect(m_path, &Model::Path::selectedChanged, this, &PathItem::selectedChanged);
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
		m_path->setSelected(isSelected());
		m_outsideSelectionBlocked = false;
	}

	return QGraphicsItem::itemChange(change, value);
}

void PathItem::selectedChanged(bool selected)
{
	if (!m_outsideSelectionBlocked) {
		setSelected(selected);
	}
}

}
