#pragma once

#include <iostream>
#include <QVector2D>

inline std::ostream &operator<<(std::ostream &stream, const QVector2D &vector)
{
	return (stream <<"(" << vector.x() << "," << vector.y() << ")");
}
