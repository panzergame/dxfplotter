#pragma once

#include <geometry/utils.h>

#include <vtkActor.h>
#include <vtkNew.h>

namespace view::view3d::internal
{

class ToolPath
{
private:
	vtkNew<vtkActor> m_actor;
	double m_boundingBox[6];

	void createPolylineFromPoints(const geometry::Point3DList &points);

public:
	ToolPath() = default;
	explicit ToolPath(const geometry::Point3DList &points);

	vtkActor *actor();
	const double (&boundingBox() const)[6];
};

}
