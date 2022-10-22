#pragma once

#include <QGraphicsItemAnimation>

namespace model
{

class Document;

class Simulation
{
private:

public:
	explicit Simulation(Document &document);

	QVector3D position(float time);
};

}
