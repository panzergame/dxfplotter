module;

#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DRender/QCamera>
#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>

export module view.simulation.internal.viewport;
import view.simulation.internal.scene;

export namespace view::simulation::internal
{

class Viewport : public Qt3DExtras::Qt3DWindow
{
private:
	void mousePressEvent(QMouseEvent* e) final
	{
		if (e->buttons() & Qt::MiddleButton) {
			m_lastMousePos = e->pos();
		}
	}

	void mouseMoveEvent(QMouseEvent* e) final
	{
		if (e->buttons() & Qt::MiddleButton) {
			const QPoint delta = computeMouseDelta(e->pos());

			const bool shiftCenter = e->modifiers() & Qt::ShiftModifier;
			if (shiftCenter) {
				const bool slowMotion = e->modifiers() & Qt::ControlModifier;
				const float factor = slowMotion ? 0.001f : 0.01f;
				const QVector3D translation(-delta.x() * factor, delta.y() * factor, 0.0f);
				camera()->translate(translation);
			} else {
				camera()->panAboutViewCenter(-delta.x());
				camera()->tiltAboutViewCenter(delta.y());
			}
		}
	}

	void wheelEvent(QWheelEvent* e) final
	{
		const QPoint numPixels = e->pixelDelta();

		const bool slowMotion = e->modifiers() & Qt::ControlModifier;
		const float factor = slowMotion ? 0.001f : 0.1f;

		const QVector3D translation(0.0f, 0.0f, numPixels.y() * factor);
		camera()->translate(translation, Qt3DRender::QCamera::DontTranslateViewCenter);
	}

	QPoint m_lastMousePos;
	QPoint computeMouseDelta(const QPoint& pos)
	{
		const QPoint delta = pos - m_lastMousePos;
		m_lastMousePos = pos;

		return delta;
	}

public:
	explicit Viewport() { defaultFrameGraph()->setClearColor(QColor(0, 0, 0)); }

	QWidget* container() { return QWidget::createWindowContainer(this); }

	void setScene(Scene* scene)
	{
		setRootEntity(scene);

		camera()->setPosition(QVector3D(0, 0, 40.0f));
		camera()->viewAll();
		camera()->setViewCenter(QVector3D(0.0f, 0.0f, 0.0f));
	}

	bool event(QEvent* e) override { return Qt3DWindow::event(e); }
};

}
