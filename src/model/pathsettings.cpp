#include <pathsettings.h>

namespace Model
{

PathSettings::PathSettings(float feedRate, float intensity, float depth)
	:m_feedRate(feedRate),
	m_intensity(intensity),
	m_depth(depth)
{
}

float PathSettings::feedRate() const
{
	return m_feedRate;
}

void PathSettings::setFeedRate(float feedRate)
{
	m_feedRate = feedRate;
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
