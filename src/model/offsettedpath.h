#pragma once

#include <geometry/polyline.h>

#include <common/aggregable.h>

#include <model/renderable.h>
#include <model/pathsettings.h>

#include <serializer/access.h>

namespace Model
{

class Layer;

class OffsettedPath : public QObject
{
	Q_OBJECT

	friend Serializer::Access<OffsettedPath>;

public:
	enum class Direction
	{
		LEFT = 0,
		RIGHT
	};

private:
	Geometry::Polyline::List m_polylines;
	Direction m_direction;

public:
	explicit OffsettedPath(Geometry::Polyline::List &&offsettedPolylines, Direction direction);
	explicit OffsettedPath() = default;

	const Geometry::Polyline::List &polylines() const;
	Geometry::CuttingDirection cuttingDirection() const;

	void transform(const QTransform &matrix);

Q_SIGNALS:
	void polylinesTransformed();
};

}
