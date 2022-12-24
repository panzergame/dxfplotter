#pragma once

#include <vtkActor.h>
#include <vtkNew.h>

namespace view::view3d::internal
{

class Tool
{
private:
	vtkNew<vtkActor> m_actor;

public:
	explicit Tool(float radius, float height);

	vtkActor *actor();
};

}
