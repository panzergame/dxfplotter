#include <pathgroupsettings.h>

#include <QDebug> // TODO

namespace Model
{

PathGroupSettings::PathGroupSettings(const Task *task)
	:m_task(task)
{
}

std::optional<float> PathGroupSettings::feedRate() const
{
	return valueIfAllEqual(&PathSettings::feedRate);
}

void PathGroupSettings::setFeedRate(float feedRate)
{
	setValue(&PathSettings::setFeedRate, feedRate);
}

std::optional<float> PathGroupSettings::intensity() const
{
	return valueIfAllEqual(&PathSettings::intensity);
}

void PathGroupSettings::setIntensity(float intensity)
{
	setValue(&PathSettings::setIntensity, intensity);
}

std::optional<float> PathGroupSettings::depth() const
{
	return valueIfAllEqual(&PathSettings::depth);
}

void PathGroupSettings::setDepth(float depth)
{
	setValue(&PathSettings::setDepth, depth);
}

}
