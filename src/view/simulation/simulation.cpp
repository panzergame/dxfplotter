#include <simulation.h>
#include <internal/viewport.h>

namespace view::simulation
{

void Simulation::documentChanged()
{
}

void Simulation::moveToolAtTime(int ms)
{
	const float seconds = float(ms) / 1e3;
	timeLabel->setText(QString("%1").arg(seconds, 0, 'f', 3));

	const model::Simulation::ToolPathPoint3D toolPosition = m_simulation.toolPositionAtTime(seconds);

	m_viewport->setToolPosition(toolPosition);
}

Simulation::Simulation(model::Application& app)
	:DocumentModelObserver(app)
{
	setupUi(this);

	connect(timeSlider, &QSlider::valueChanged, this, &Simulation::moveToolAtTime);
}

Simulation::~Simulation() = default;

void Simulation::setSimulation(model::Simulation && simulation)
{
	m_simulation = std::move(simulation);

	m_viewport = std::make_unique<internal::Viewport>(m_simulation);
	container->addWidget(m_viewport.get());

	const float secondDuration = m_simulation.duration();
	timeSlider->setMaximum(secondDuration * 1e3);
}

}
