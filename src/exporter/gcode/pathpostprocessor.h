#pragma once

#include <exporter/gcode/postprocessor.h>

using namespace fmt::literals;

namespace exporter::gcode
{

/** @brief Augmented post processor including path settings variables such as feed rate and intensity
 */
class PathPostProcessor : public PostProcessor
{
private:
	const model::PathSettings &m_settings;

	template <class ...Args>
	void printWithSettings(const std::string &format, Args&& ...args)
	{
		print(format, std::forward<Args>(args)...,
			  "S"_a=m_settings.intensity());
	}

public:
	explicit PathPostProcessor(const model::PathSettings &settings, const config::Tools::Tool& tool, const config::Profiles::Profile::Gcode& gcode, std::ostream &stream);

	void preCut();
	void planeLinearMove(const Eigen::Vector2d &to);
	void depthLinearMove(float depth);
	void cwArcMove(const Eigen::Vector2d &relativeCenter, const Eigen::Vector2d &to);
	void ccwArcMove(const Eigen::Vector2d &relativeCenter, const Eigen::Vector2d &to);
};

}

