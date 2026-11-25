module;

#include <cstdint>

export module model.pathsettings;
import serializer.access;

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
	float m_depthOffset;

public:
	explicit PathSettings() = default;
	explicit PathSettings(float planeFeedRate, float depthFeedRate, float intensity, float depth,
						  float depthOffset)
		: m_planeFeedRate(planeFeedRate)
		, m_depthFeedRate(depthFeedRate)
		, m_intensity(intensity)
		, m_depth(depth)
		, m_depthOffset(depthOffset)
	{
	}

	float planeFeedRate() const { return m_planeFeedRate; }

	void setPlaneFeedRate(float planeFeedRate) { m_planeFeedRate = planeFeedRate; }

	float depthFeedRate() const { return m_depthFeedRate; }

	void setDepthFeedRate(float depthFeedRate) { m_depthFeedRate = depthFeedRate; }

	float intensity() const { return m_intensity; }

	void setIntensity(float intensity) { m_intensity = intensity; }

	float depth() const { return m_depth; }

	void setDepth(float depth) { m_depth = depth; }

	float depthOffset() const { return m_depthOffset; }

	void setDepthOffset(float depthOffset) { m_depthOffset = depthOffset; }
};

}
