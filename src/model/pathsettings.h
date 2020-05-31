#pragma once

namespace Model
{

class PathSettings
{
private:
	float m_feedRate;
	float m_intensity;
	int m_passes;

public:
	explicit PathSettings() = default;
	explicit PathSettings(float feedRate, float intensity, int passes);

	float feedRate() const;
	void setFeedRate(float feedRate);

	float intensity() const;
	void setIntensity(float intensity);

	int passes() const;
	void setPasses(int passes);
};

}
