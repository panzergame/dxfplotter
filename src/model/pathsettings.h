#pragma once

namespace Model
{

class PathSettings
{
private:
	float m_feedRate;
	float m_intensity;
	float m_depth;

public:
	explicit PathSettings() = default;
	explicit PathSettings(float feedRate, float intensity, float depth);

	float feedRate() const;
	void setFeedRate(float feedRate);

	float intensity() const;
	void setIntensity(float intensity);

	float depth() const;
	void setDepth(float depth);
};

}
