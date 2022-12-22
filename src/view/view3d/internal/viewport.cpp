#include <viewport.h>

#include <vtkNamedColors.h>

namespace view::view3d::internal
{

Viewport::Viewport()
{
	vtkNew<vtkNamedColors> colors;
	m_renderer->SetBackground(0.0, 0.0, 0.0);
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

