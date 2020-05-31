#include <pathsettings.h>

namespace Model
{

PathSettings::PathSettings(float feedRate, float intensity, int passes)
	:m_feedRate(feedRate),
	m_intensity(intensity),
	m_passes(passes)
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

int PathSettings::passes() const
{
	return m_passes;
}

void PathSettings::setPasses(int passes)
{
	m_passes = passes;
}

}
