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
	explicit PathSettings(float planeFeedRate, float depthFeedRate, float intensity, float depth)
		: m_planeFeedRate(planeFeedRate)
		, m_depthFeedRate(depthFeedRate)
		, m_intensity(intensity)
		, m_depth(depth)
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
};

}
