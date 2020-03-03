#include <line.h>
#include <utils.h>

#include <QDebug> // TODO

namespace Importer::Dxf
{

Geometry::Polyline::List convertToPolylines(const DRW_LWPolyline &lwpolyline)
{
	if (lwpolyline.vertlist.size() <= 1) {
		return {};
	}

	Geometry::Bulge::List bulges(lwpolyline.vertlist.size() - 1);

	DRW_Vertex2D *vertex = lwpolyline.vertlist.front();
	QVector2D start(vertex->x, vertex->y);

	for (int i = 1, size = lwpolyline.vertlist.size(); i < size; ++i) {
		DRW_Vertex2D *nextVertex = lwpolyline.vertlist[i];
		const QVector2D end(nextVertex->x, nextVertex->y);

		bulges[i - 1] = Geometry::Bulge(start, end, vertex->bulge);

		// Pass to next vertex begin
		vertex = nextVertex;
		start = end;
	}

	return {Geometry::Polyline(std::move(bulges))};
}

};
