#pragma once

#include <control/application.h>

#include <view/rubberband.h>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QMouseEvent>

namespace View
{

class Viewport : public QGraphicsView
{
private:
	Control::Application &m_app;
	/// List of selected path.
	Model::Path::ListPtr m_selectedPaths;
	/// Last mouse position in scene.
	QPoint m_lastMousePosition;

	RubberBand m_rubberBand;

	void setupPathItems();
	void setupAxes();
	void setupHighlights();

	void startMovement(const QPoint &mousePos);
	void updateMovement(const QPoint &mousePos);

	void startRubberBand(const QPoint &mousePos);
	void updateRubberBand(const QPoint &mousePos);
	void endRubberBand(const QPoint &mousePos, bool addToSelection);

protected Q_SLOTS:
	void wheelEvent(QWheelEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;

public:
	explicit Viewport(Control::Application &app);
};

}
