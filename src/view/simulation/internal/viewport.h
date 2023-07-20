#pragma once

#include <Qt3DExtras/Qt3DWindow>

class QMouseEvent;
class QWheelEvent;

namespace view::simulation::internal
{

class Scene;

class Viewport : public Qt3DExtras::Qt3DWindow
{
private:
	void mousePressEvent(QMouseEvent *e) final;
	void mouseMoveEvent(QMouseEvent *e) final;
	void wheelEvent(QWheelEvent *e) final;

	QPoint m_lastMousePos;
	QPoint computeMouseDelta(const QPoint& pos);

public:
	explicit Viewport();

	QWidget *container();

	void setScene(Scene *scene);
	
	bool event(QEvent *e) override;
};

}
