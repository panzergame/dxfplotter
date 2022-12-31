#include <tool.h>

#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkNamedColors.h>
#include <vtkProperty.h>
#include <vtkColor.h>

namespace view::simulation::internal
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

void Tool::setPosition(const model::Simulation::ToolPathPoint3D& position)
{
	const QVector3D& pos = position.position;
	m_actor->SetPosition(pos.x(), pos.y(), pos.z());

	static vtkNew<vtkNamedColors> namedColors;

	static vtkColor3d colorsByMoveType[] = {
		namedColors->GetColor3d("AliceBlue"), // MoveType::NormalWithCut
		namedColors->GetColor3d("DimGray") // MoveType::FastWithoutCut
	};

	m_actor->GetProperty()->SetColor(colorsByMoveType[static_cast<int>(position.moveType)].GetData());
}

}
