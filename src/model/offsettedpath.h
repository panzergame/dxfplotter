#pragma once

#include <model/renderable.h>
#include <model/pathsettings.h>

#include <serializer/access.h>

import common.aggregable;
import geometry.polyline;
import geometry.rect;
import geometry.utils;

namespace model
{

class Layer;

class OffsettedPath : public QObject
{
	Q_OBJECT

	friend serializer::Access<OffsettedPath>;

public:
	enum class Direction { LEFT = 0, RIGHT };

private:
	geometry::Polyline::List m_polylines;
	Direction m_direction;

public:
	explicit OffsettedPath(geometry::Polyline::List&& offsettedPolylines, Direction direction);
	explicit OffsettedPath(const OffsettedPath& other);
	explicit OffsettedPath() = default;

	const geometry::Polyline::List& polylines() const;
	geometry::CuttingDirection cuttingDirection() const;

	void transform(const QTransform& matrix);

	geometry::Rect boundingRect() const;

Q_SIGNALS:
	void polylinesTransformed();
};

}
