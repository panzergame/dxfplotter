#include <viewport.h>
#include <scene.h>

#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DRender/QCamera>

#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>

namespace view::simulation::internal
{

void Viewport::mousePressEvent(QMouseEvent *e)
{
	if (e->buttons() & Qt::MiddleButton) {
		m_lastMousePos = e->pos();
	}
}

void Viewport::mouseMoveEvent(QMouseEvent *e)
{
	if (e->buttons() & Qt::MiddleButton) {
		const QPoint delta = computeMouseDelta(e->pos());

		const bool shiftCenter = e->modifiers() & Qt::ShiftModifier;
		if (shiftCenter) {
			const bool slowMotion = e->modifiers() & Qt::ControlModifier;
			const float factor = slowMotion ? 0.001f : 0.01f;
			const QVector3D translation(-delta.x() * factor, delta.y() * factor, 0.0f);
			camera()->translate(translation);
		}
		else {
			camera()->panAboutViewCenter(-delta.x());
			camera()->tiltAboutViewCenter(delta.y());
		}
	}
}

void Viewport::wheelEvent(QWheelEvent *e)
{
	const QPoint numPixels = e->pixelDelta();
	
	const bool slowMotion = e->modifiers() & Qt::ControlModifier;
	const float factor = slowMotion ? 0.001f : 0.01f;

	const QVector3D translation(0.0f, 0.0f, numPixels.y() * factor);
	camera()->translate(translation, Qt3DRender::QCamera::DontTranslateViewCenter);
}

QPoint Viewport::computeMouseDelta(const QPoint &pos)
{
	const QPoint delta = pos - m_lastMousePos;
	m_lastMousePos = pos;

	return delta;
}

Viewport::Viewport()
{
	defaultFrameGraph()->setClearColor(QColor(0, 0, 0));
}

QWidget *Viewport::container()
{
	return QWidget::createWindowContainer(this);
}

void Viewport::setScene(Scene *scene)
{
	setRootEntity(scene);

    camera()->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    camera()->setPosition(QVector3D(0, 0, 40.0f));
	camera()->viewAll();
	camera()->setViewCenter(QVector3D(0.0f, 0.0f, 0.0f));
	qInfo() << camera()->viewCenter();
}

bool Viewport::event(QEvent *e)
{
	return Qt3DWindow::event(e);
}

}
