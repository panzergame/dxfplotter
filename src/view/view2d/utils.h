#pragma once

#include <QPointF>
#include <Eigen/Dense>

namespace view::view2d
{

inline QPointF toPointF(const Eigen::Vector2d &vector)
{
	return {vector.x(), vector.y()};
}

}
