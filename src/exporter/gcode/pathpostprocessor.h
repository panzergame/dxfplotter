#pragma once

#include <exporter/gcode/postprocessor.h>

namespace Exporter::GCode
{

/** @brief Augmented post processor including path settings variables such as feed rate and intensity
 */
class PathPostProcessor : public PostProcessor
{
private:
	const Model::PathSettings &m_settings;

	template <class ...Args>
	void printWithSettings(const std::string &format, Args&& ...args)
	{
		print(format, std::forward<Args>(args)...,
			  "F"_a=m_settings.feedRate(),
			  "S"_a=m_settings.intensity());
	}

public:
	explicit PathPostProcessor(const Model::PathSettings &settings, const Format &format, std::ostream &stream);

	void toolOn();
	void linearMove(const QVector2D &to);
	void cwArc(const QVector2D &relativeCenter, const QVector2D &to);
	void ccwArc(const QVector2D &relativeCenter, const QVector2D &to);
};

}

