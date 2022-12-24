#include <tool.h>

#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>

namespace view::view3d::internal
{

Tool::Tool(float radius, float height)
{
	vtkNew<vtkCylinderSource> cylinderSource;
	cylinderSource->SetCenter(0.0, height / 2.0, 0.0);
	cylinderSource->SetRadius(radius);
	cylinderSource->SetHeight(height);
	cylinderSource->SetResolution(64);

	vtkNew<vtkPolyDataMapper> mapper;
	mapper->SetInputConnection(cylinderSource->GetOutputPort());
	m_actor->SetMapper(mapper);
	m_actor->RotateX(90);
}

vtkActor *Tool::actor()
{
	return m_actor;
}

}
