#pragma once

#include <model/path.h>
#include <exporter/gcode/exporter.h>

#include <fmt/format.h>
#include <sstream>

namespace Exporter::GCode
{

using namespace fmt::literals;

class PostProcessor
{
private:
	const Model::PathSettings &m_settings;
	const Format &m_format;
	std::ostream &m_stream;

public:
	explicit PostProcessor(const Model::PathSettings &settings, const Format &format, std::ostream &stream);

	template <class ...Args>
	void print(const std::string &format, Args&& ...args)
	{
		m_stream << fmt::format(format, std::forward<Args>(args)...) << "\n";
	}

	template <class ...Args>
	void printCommand(const std::string &format, Args&& ...args)
	{
		print(format, std::forward<Args>(args)...,
			  "F"_a=m_settings.feedRate(),
			  "S"_a=m_settings.intensity());
	}

	void toolOn();
	void toolOff();
	void linearMove(const QVector2D &to);
	void fastMove(const QVector2D &to);
	void cwArc(const QVector2D &relativeCenter, const QVector2D &to);
	void ccwArc(const QVector2D &relativeCenter, const QVector2D &to);
};

}

