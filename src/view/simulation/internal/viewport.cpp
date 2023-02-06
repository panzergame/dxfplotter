#include <viewport.h>
#include <scene.h>

#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DRender/QCamera>

#include <QWidget>

namespace view::simulation::internal
{

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

	camera()->viewAll();

	Qt3DExtras::QOrbitCameraController *camController = new Qt3DExtras::QOrbitCameraController(scene);
	camController->setLinearSpeed( 200.0f );
	camController->setLookSpeed( 200.0f );
	camController->setCamera(camera());

}

}
