#include <rubberband.h>

#include <QPainter>
#include <QDebug>

namespace view::view2d
{

static const QBrush borderBrush(QColor(0, 0, 255, 255));
static const QBrush fillBrush(QColor(0, 0, 255, 100));
static const QPen borderPen(borderBrush, 0.0f);

void RubberBand::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->setPen(borderPen);
	painter->drawRect(m_rectf);
	painter->fillRect(m_rectf, fillBrush);
}

QRectF RubberBand::boundingRect() const
{
	return rect();
}

QRectF RubberBand::rect() const
{
	return m_rectf.normalized();
}

bool RubberBand::empty(int tolerance) const
{
	const QRect normalizedRect = m_rect.normalized();
	return (normalizedRect.width() < tolerance) && (normalizedRect.height() < tolerance);
}

void RubberBand::start(const QPoint& screenStartCorner, const QPointF &sceneStartCorner)
{
	setVisible(true);
	prepareGeometryChange();

	m_rect = QRect(screenStartCorner, screenStartCorner);
	m_rectf = QRectF(sceneStartCorner, sceneStartCorner);
}

void RubberBand::update(const QPoint& screenEndCorner, const QPointF &sceneEndCorner)
{
	prepareGeometryChange();

	m_rect.setBottomRight(screenEndCorner);
	m_rectf.setBottomRight(sceneEndCorner);
}

void RubberBand::end(const QPoint& screenEndCorner, const QPointF &sceneEndCorner)
{
	update(screenEndCorner, sceneEndCorner);

	setVisible(false);
}

}
