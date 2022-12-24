#include <viewport.h>

#include <vtkAxesActor.h>

namespace view::view3d::internal
{

Viewport::Viewport()
{
	m_renderer->SetBackground(0.0, 0.0, 0.0);

	vtkNew<vtkAxesActor> axes;
	axes->AxisLabelsOff();
	m_renderer->AddActor(axes);
}

void Viewport::addActor(vtkActor* actor)
{
	m_renderer->AddActor(actor);
}

void Viewport::resetCamera(const double bounds[6])
{
	m_renderer->ResetCamera(bounds);
}

vtkRenderer *Viewport::renderer() const
{
	return m_renderer;
}


}

