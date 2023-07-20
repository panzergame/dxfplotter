#pragma once

#include <model/simulation.h>

#include "uic/simulation/ui_simulation.h"

#include <QWidget>
#include <QTimer>

namespace view::simulation
{

namespace internal
{

class Scene;
class Viewport;

}

class Simulation : private Ui::Simulation, public QWidget
{
private:
	model::Simulation m_simulation;

	std::unique_ptr<internal::Viewport> m_viewport;
	std::unique_ptr<internal::Scene> m_scene;

	QTimer m_timer;

protected slots:
	void moveToolAtTime(int ms);
	void startStopToolAnimation();

public:
	explicit Simulation();
	~Simulation();

	void setSimulation(model::Simulation&& simulation);
};

}
