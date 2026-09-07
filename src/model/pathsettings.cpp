module;

#include <cstdint>
#include <serializer/access.h>

export module model.pathsettings;

export namespace model
{

class PathSettings
{
	friend serializer::Access<PathSettings>;

private:
	float m_planeFeedRate;
	float m_depthFeedRate;
	float m_intensity;
	float m_depth;

public:
	explicit PathSettings() = default;
	explicit PathSettings(float planeFeedRate, float depthFeedRate, float intensity, float depth);

	float planeFeedRate() const;
	void setPlaneFeedRate(float planeFeedRate);

	float depthFeedRate() const;
	void setDepthFeedRate(float depthFeedRate);

	float intensity() const;
	void setIntensity(float intensity);

	float depth() const;
	void setDepth(float depth);
};

}

namespace model
{

PathSettings::PathSettings(float planeFeedRate, float depthFeedRate, float intensity, float depth)
	: m_planeFeedRate(planeFeedRate)
	, m_depthFeedRate(depthFeedRate)
	, m_intensity(intensity)
	, m_depth(depth)
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
