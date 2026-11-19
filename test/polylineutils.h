#pragma once

#include <QVector2D>

import geometry.polyline;

geometry::Polyline createStartPolyline(float innerRadius, float outterRadius, int nbBranches);

/// Counter-clockwise square of side size whose bottom left corner is min.
geometry::Polyline createSquarePolyline(const QVector2D& min, float size);
