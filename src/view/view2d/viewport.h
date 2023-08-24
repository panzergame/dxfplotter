#pragma once

#include <model/task.h>
#include <model/documentmodelobserver.h>

#include <view/view2d/rubberband.h>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QKeyEvent>

namespace view::view2d
{

class Viewport : public model::DocumentModelObserver<QGraphicsView>
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

	void selectAllItems();
	void deselecteAllItems();

	void setupModel();

	void fitItemsInView();

protected:
	void documentChanged() override;
	void newDocumentOpened() override;

	void wheelEvent(QWheelEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;
	void drawBackground(QPainter *painter, const QRectF &updatedRect) override;

public:
	explicit Viewport(model::Application &app);

Q_SIGNALS:
	void cursorMoved(const QPointF &position);
};

}
