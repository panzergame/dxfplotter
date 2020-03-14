#pragma once

#include <control/application.h>

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

	void setupPathItems();
	void setupAxes();
	void setupHighlights();

protected Q_SLOTS:
	void wheelEvent(QWheelEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void selectionChanged();

public:
	explicit Viewport(Control::Application &app);
};

}
