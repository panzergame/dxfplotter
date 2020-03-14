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

	void setupPathItems();
	void setupAxes();
	void setupHighlights();

protected Q_SLOTS:
	void wheelEvent(QWheelEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;

public:
	explicit Viewport(Control::Application &app);
};

}
