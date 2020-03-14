#include <pathsettings.h>

namespace Model
{

PathSettings::PathSettings(float feedRate, float intensity)
	:m_feedRate(feedRate),
	m_intensity(intensity)
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

}
