#pragma once

#include <vtkActor.h>
#include <vtkNew.h>

#include <model/simulation.h>

namespace view::simulation::internal
{

class Tool
{
private:
	vtkNew<vtkActor> m_actor;

public:
	explicit Tool(float radius, float height);

	vtkActor *actor();

	void setPosition(const model::Simulation::ToolPathPoint3D& position);
};

}
