#pragma once

namespace Model
{

class PathSettings
{
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
