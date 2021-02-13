#include <pathgroupsettings.h>

#include <QDebug> // TODO

namespace Model
{

PathGroupSettings::PathGroupSettings(const Task *task)
	:m_task(task)
{
}

std::optional<float> PathGroupSettings::planeFeedRate() const
{
	return valueIfAllEqual(&PathSettings::planeFeedRate);
}

void PathGroupSettings::setPlaneFeedRate(float planeFeedRate)
{
	setValue(&PathSettings::setPlaneFeedRate, planeFeedRate);
}

std::optional<float> PathGroupSettings::depthFeedRate() const
{
	return valueIfAllEqual(&PathSettings::depthFeedRate);
}

void PathGroupSettings::setDepthFeedRate(float depthFeedRate)
{
	setValue(&PathSettings::setDepthFeedRate, depthFeedRate);
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
