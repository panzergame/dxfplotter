#include <simulation.h>
#include <internal/scene.h>
#include <internal/viewport.h>

#include <QDate>

namespace view::simulation
{

static void setLabelTimeText(QLabel *label, int ms)
{
	const QString timeText = QTime::fromMSecsSinceStartOfDay(ms).toString("hh:mm:ss:zzz");
	label->setText(timeText);
}

void Simulation::moveToolAtTime(int ms)
{
	setLabelTimeText(timeLabel, ms);

	const float seconds = float(ms) / 1e3;
	const model::Simulation::ToolPathPoint3D toolPosition = m_simulation.toolPositionAtTime(seconds);

	m_scene->setToolPosition(toolPosition);
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

Simulation::Simulation()
	:m_viewport(new internal::Viewport())
{
	setupUi(this);

	connect(timeSlider, &QSlider::valueChanged, this, &Simulation::moveToolAtTime);
	connect(startStopButton, &QPushButton::clicked, this, &Simulation::startStopToolAnimation);
	
	addAction(actionPauseResume);

	container->addWidget(m_viewport->container());

	static const int timerBaseIntervalMs = 30;
	m_timer.setInterval(timerBaseIntervalMs);
	m_timer.callOnTimeout([this](){
		const int timerIntervalMs = timerBaseIntervalMs * speedSpinBox->value();
		timeSlider->setSliderPosition(timeSlider->sliderPosition() + timerIntervalMs);
	});
}

Simulation::~Simulation() = default;

void Simulation::setSimulation(model::Simulation && simulation)
{
	m_simulation = std::move(simulation);

	internal::Scene *newScene = new internal::Scene(m_simulation);
	m_viewport->setScene(newScene);
	m_scene.reset(newScene);

	const float secondDuration = m_simulation.duration();
	const int durationMs = secondDuration * 1e3;
	timeSlider->setMaximum(durationMs);

	setLabelTimeText(totalTimeLabel, durationMs);
}

}
