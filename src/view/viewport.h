#pragma once

#include <model/task.h>
#include <model/taskmodelobserver.h>

#include <view/rubberband.h>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QMouseEvent>

namespace View
{

class Viewport : public Model::TaskModelObserver<QGraphicsView>
{
	Q_OBJECT;

private:
	/// Last mouse position in scene.
	QPoint m_lastMousePosition;

	RubberBand m_rubberBand;

	void setupPathItems();

	void startMovement(const QPoint &mousePos);
	void updateMovement(const QPoint &mousePos);

	void startRubberBand(const QPoint &mousePos);
	void updateRubberBand(const QPoint &mousePos);
	void endRubberBand(const QPoint &mousePos, bool addToSelection);

	void setupModel();

protected:
	void taskChanged() override;

	void wheelEvent(QWheelEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void drawBackground(QPainter *painter, const QRectF &updatedRect) override;

public:
	explicit Viewport(Model::Application &app);

Q_SIGNALS:
	void cursorMoved(const QPointF &position);
};

}
