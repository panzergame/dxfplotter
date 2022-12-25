#pragma once

#include <model/documentmodelobserver.h>
#include <model/simulation.h>

#include "uic/simulation/ui_simulation.h"

#include <QWidget>
#include <QTimer>

namespace view::simulation
{

namespace internal
{

class Viewport;

}

class Simulation : private Ui::Simulation, public model::DocumentModelObserver<QWidget>
{
private:
	model::Simulation m_simulation;
	std::unique_ptr<internal::Viewport> m_viewport;
	QTimer m_timer;

protected:
	void documentChanged() override;

protected slots:
	void moveToolAtTime(int ms);

public:
	explicit Simulation(model::Application &app);
	~Simulation();

	void setSimulation(model::Simulation&& simulation);
};

}
