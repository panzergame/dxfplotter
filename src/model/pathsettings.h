#pragma once

namespace Model
{

class PathSettings
{
private:
	float m_feedRate;
	float m_intensity;

public:
	explicit PathSettings() = default;
	explicit PathSettings(float feedRate, float intensity);

	float feedRate() const;
	void setFeedRate(float feedRate);

	float intensity() const;
	void setIntensity(float intensity);
};

}
