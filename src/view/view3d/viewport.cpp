#include <viewport.h>

#include <internal/toolpath.h>
#include <internal/viewport.h>

#include <QResizeEvent>
#include <QVTKOpenGLNativeWidget.h>

#include <vtkGenericOpenGLRenderWindow.h>

namespace view::view3d
{

void Viewport::documentChanged()
{
}

void Viewport::resizeEvent(QResizeEvent *event)
{
	m_vtkWidget->resize(event->size());
}

Viewport::Viewport(model::Application& app)
	:DocumentModelObserver(app),
	m_vtkWidget(new QVTKOpenGLNativeWidget(this))
{
	vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
	m_vtkWidget->setRenderWindow(renderWindow);

	m_vtkWidget->show();
}

void Viewport::setSimulation(model::Simulation && simulation)
{
	vtkRenderWindow *window = m_vtkWidget->renderWindow();
	if (m_viewport) {
		window->RemoveRenderer(m_viewport->renderer());
	}

	m_simulation = std::move(simulation);
	m_path = std::make_unique<internal::ToolPath>(m_simulation.approximatedPathToLines(0.01)); // TODO maxError

	m_viewport = std::make_unique<internal::Viewport>();
	m_viewport->addActor(m_path->actor());
	m_viewport->resetCamera(m_path->boundingBox());

	window->AddRenderer(m_viewport->renderer());
	window->Render();
}

}
