#pragma once

#include <vtkRenderer.h>
#include <vtkNew.h>

class vtkActor;

namespace view::view3d::internal
{

class Viewport
{
private:
	vtkNew<vtkRenderer> m_renderer;

public:
	explicit Viewport();

	void addActor(vtkActor *actor);
	void resetCamera(const double bounds[6]);
	vtkRenderer *renderer() const;
};

}
