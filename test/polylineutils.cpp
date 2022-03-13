#include <polylineutils.h>

geometry::Polyline createStartPolyline(float innerRadius, float outterRadius, int nbBranches)
{
	geometry::Point2DList points(nbBranches * 2);

	for (int i = 0, max = nbBranches * 2; i < max; i += 2) {
		const float angle = M_PI * i / nbBranches;
		points[i] = QVector2D(std::cos(angle), std::sin(angle)) * outterRadius;
	}

	for (int i = 1, max = nbBranches * 2; i < max; i += 2) {
		const float angle = M_PI * i / nbBranches;
		points[i] = QVector2D(std::cos(angle), std::sin(angle)) * innerRadius;
	}

	geometry::Bulge::List bulges(points.size());
	for (int size = points.size(), i = (size - 1), j = 0; j < size; i = j++) {
		bulges[j] = geometry::Bulge(points[i], points[j], 0.0f);
	}

	return geometry::Polyline(std::move(bulges));
}
