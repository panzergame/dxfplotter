#include <pathsettings.h>

namespace Model
{

PathSettings::PathSettings(float planeFeedRate, float depthFeedRate, float intensity, float depth)
	:m_planeFeedRate(planeFeedRate),
	m_depthFeedRate(depthFeedRate),
	m_intensity(intensity),
	m_depth(depth)
{
}

float PathSettings::planeFeedRate() const
{
	return m_planeFeedRate;
}

void PathSettings::setPlaneFeedRate(float planeFeedRate)
{
	m_planeFeedRate = planeFeedRate;
}

float PathSettings::depthFeedRate() const
{
	return m_depthFeedRate;
}

void PathSettings::setDepthFeedRate(float depthFeedRate)
{
	m_depthFeedRate = depthFeedRate;
}

float PathSettings::intensity() const
{
	return m_intensity;
}

void PathSettings::setIntensity(float intensity)
{
	m_intensity = intensity;
}

float PathSettings::depth() const
{
	return m_depth;
}

void PathSettings::setDepth(float depth)
{
	m_depth = depth;
}

}
