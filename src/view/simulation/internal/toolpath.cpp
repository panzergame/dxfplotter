#include <toolpath.h>

#include <vtkNamedColors.h>
#include <vtkProperty.h>
#include <vtkPoints.h>
#include <vtkLine.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>

#include <QDebug> // TODO

namespace view::simulation::internal
{

static vtkNew<vtkNamedColors> namedColors;

void ToolPath::createPolylineFromPoints(const model::Simulation::ToolPathPoint3D::List& points)
{
	const int nbPoints = points.size();
	const int nbLines = nbPoints - 1;

	vtkNew<vtkPoints> vertices;
	vertices->SetNumberOfPoints(nbPoints);

	for (int i = 0; i < nbPoints; ++i) {
		const model::Simulation::ToolPathPoint3D &point = points[i];
		const QVector3D& position = point.position;
		vertices->SetPoint(i, position.x(), position.y(), position.z());
	}

	vtkNew<vtkUnsignedCharArray> colors;
	colors->SetNumberOfComponents(3);
	colors->SetNumberOfTuples(nbLines);

	static const vtkColor3ub colorsByMoveType[] = {
		namedColors->GetColor3ub("OrangeRed"), // MoveType::NormalWithCut
		namedColors->GetColor3ub("SlateBlue") // MoveType::FastWithoutCut
	};

	vtkNew<vtkCellArray> cells;
	for (int i = 0; i < nbLines; ++i) {
		const model::Simulation::ToolPathPoint3D &secondPoint = points[i + 1];

		colors->SetTypedTuple(i, colorsByMoveType[static_cast<int>(secondPoint.moveType)].GetData());

		vtkNew<vtkLine> line;
		line->GetPointIds()->SetId(0, i);
		line->GetPointIds()->SetId(1, i + 1);
		cells->InsertNextCell(line);
	}

	vertices->GetBounds(m_boundingBox);

	vtkNew<vtkPolyData> polyData;
	polyData->SetPoints(vertices);
	polyData->SetLines(cells);
    polyData->GetCellData()->SetScalars(colors);

	vtkNew<vtkPolyDataMapper> mapper;
	mapper->SetInputData(polyData);

	m_actor->SetMapper(mapper);
}

ToolPath::ToolPath(const model::Simulation::ToolPathPoint3D::List& points)
{
	createPolylineFromPoints(points);
}

vtkActor *ToolPath::actor()
{
	return m_actor;
}

const double (&ToolPath::boundingBox() const)[6]
{
	return m_boundingBox;
}


}
