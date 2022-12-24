#pragma once

#include <model/simulation.h>

#include <vtkActor.h>
#include <vtkNew.h>

namespace view::view3d::internal
{

class ToolPath
{
private:
	vtkNew<vtkActor> m_actor;
	double m_boundingBox[6];

	void createPolylineFromPoints(const model::Simulation::ToolPathPoint3D::List &points);

public:
	ToolPath() = default;
	explicit ToolPath(const model::Simulation::ToolPathPoint3D::List &points);

	vtkActor *actor();
	const double (&boundingBox() const)[6];
};

}
