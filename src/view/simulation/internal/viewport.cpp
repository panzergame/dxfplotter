#include <viewport.h>

#include <vtkAxesActor.h>
#include <vtkRenderer.h>
#include <vtkNew.h>
#include <vtkGenericOpenGLRenderWindow.h>

#include <QResizeEvent>

namespace view::simulation::internal
{

Viewport::Viewport(const model::Simulation& simulation)
	:m_tool(simulation.toolRadius(), 1.0f),
	m_toolPath(simulation.approximatedToolPathToLines(0.01))
{
	vtkNew<vtkRenderer> renderer;
	renderer->SetBackground(0.0, 0.0, 0.0);

	renderer->AddActor(m_tool.actor());
	renderer->AddActor(m_toolPath.actor());

	vtkNew<vtkAxesActor> axes;
	axes->AxisLabelsOff();
	renderer->AddActor(axes);

	vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
	renderWindow->AddRenderer(renderer);
	renderer->ResetCamera(m_toolPath.boundingBox());
	setRenderWindow(renderWindow);
}

void Viewport::setToolPosition(const model::Simulation::ToolPathPoint3D& position)
{
	m_tool.setPosition(position);
	renderWindow()->Render();
}

}

