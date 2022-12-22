#include <toolpath.h>

#include <vtkNamedColors.h>
#include <vtkProperty.h>
#include <vtkPoints.h>
#include <vtkPolyLine.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>

#include <QDebug> // TODO

namespace view::view3d::internal
{

static vtkNew<vtkNamedColors> colors;

void ToolPath::createPolylineFromPoints(const geometry::Point3DList& points)
{
	const int nbPoints = points.size();

	vtkNew<vtkPoints> vertices;
	vertices->SetNumberOfPoints(nbPoints);

	vtkNew<vtkPolyLine> polyline;
	vtkIdList *ids = polyline->GetPointIds();
	ids->SetNumberOfIds(nbPoints);

	for (int i = 0; i < nbPoints; ++i) {
		const QVector3D& point = points[i];
		vertices->SetPoint(i, point.x(), point.y(), point.z());

		ids->SetId(i, i);
	}

	vertices->GetBounds(m_boundingBox);

	vtkNew<vtkCellArray> cells;
	cells->InsertNextCell(polyline);

	vtkNew<vtkPolyData> polyData;
	polyData->SetPoints(vertices);
	polyData->SetLines(cells);

	vtkNew<vtkPolyDataMapper> mapper;
	mapper->SetInputData(polyData);

	m_actor->SetMapper(mapper);
	m_actor->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());
}

ToolPath::ToolPath(const geometry::Point3DList& points)
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
