#include <line.h>
#include <utils.h>

#include <QDebug> // TODO

namespace Importer::Dxf
{

Geometry::Polyline::List convertToPolylines(const DRW_LWPolyline &lwpolyline)
{
	const int size = lwpolyline.vertlist.size();
	if (size <= 1) {
		return {};
	}

	const bool opened = !(lwpolyline.flags & (1 << 0));

	// One bulge more if openeded polyline, to connect last vertex to first vertex.
	Geometry::Bulge::List bulges(size - ((int)opened));

	DRW_Vertex2D *firstVertex = lwpolyline.vertlist.front();
	DRW_Vertex2D *vertex = firstVertex;
	QVector2D start(vertex->x, vertex->y);

	for (int i = 1; i < size; ++i) {
		DRW_Vertex2D *nextVertex = lwpolyline.vertlist[i];
		const QVector2D end(nextVertex->x, nextVertex->y);

		bulges[i - 1] = Geometry::Bulge(start, end, vertex->bulge);

		// Pass to next vertex begin
		vertex = nextVertex;
		start = end;
	}

	// Create end to start bulge if closed polyline.
	if (!opened) {
		const QVector2D end(firstVertex->x, firstVertex->y);
		bulges.back() = Geometry::Bulge(start, end, 0.0f);
	}

	return {Geometry::Polyline(std::move(bulges))};
}

};
