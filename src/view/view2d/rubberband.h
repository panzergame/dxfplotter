#pragma once

#include <QGraphicsItem>

namespace View::View2d
{

class RubberBand : public QGraphicsItem
{
private:
	QRect m_rect;
	QRectF m_rectf;

public:
	explicit RubberBand() = default;

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			QWidget *widget) override;
	QRectF boundingRect() const override;

	QRectF rect() const;
	bool empty(int tolerance) const;

	void start(const QPoint& screenStartCorner, const QPointF &sceneStartCorner);
	void update(const QPoint& screenEndCorner, const QPointF &sceneEndCorner);
	void end(const QPoint& screenEndCorner, const QPointF &sceneEndCorner);
};

}
