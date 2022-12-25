#include <simulation.h>
#include <internal/viewport.h>

#include <QDate>

namespace view::simulation
{

void Simulation::documentChanged()
{
}

void Simulation::moveToolAtTime(int ms)
{
	const QString timeText = QTime::fromMSecsSinceStartOfDay(ms).toString("hh:mm:ss:zzz");
	timeLabel->setText(timeText);

	const float seconds = float(ms) / 1e3;
	const model::Simulation::ToolPathPoint3D toolPosition = m_simulation.toolPositionAtTime(seconds);

	m_viewport->setToolPosition(toolPosition);
}

void Simulation::startStopToolAnimation()
{
	if (m_timer.isActive()) {
		m_timer.stop();
		startStopButton->setIcon(QIcon(":/icons/playback-start.svg"));
	}
	else {
		m_timer.start();
		startStopButton->setIcon(QIcon(":/icons/playback-pause.svg"));
	}
}

Simulation::Simulation(model::Application& app)
	:DocumentModelObserver(app)
{
	setupUi(this);

	connect(timeSlider, &QSlider::valueChanged, this, &Simulation::moveToolAtTime);
	connect(startStopButton, &QPushButton::clicked, this, &Simulation::startStopToolAnimation);

	static const int timerIntervalMs = 30;
	m_timer.setInterval(timerIntervalMs);
	m_timer.callOnTimeout([this](){
		timeSlider->setSliderPosition(timeSlider->sliderPosition() + timerIntervalMs);
	});
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
