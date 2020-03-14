#pragma once

#include <model/path.h>
#include <fmt/format.h>
#include <sstream>

namespace Exporter::GCode
{

using namespace fmt::literals;

class PostProcessor
{
private:
	const Model::PathSettings &m_settings;
	std::stringstream &m_stream;

public:
	explicit PostProcessor(const Model::PathSettings &settings, std::stringstream &stream);

	template <class ...Args>
	void print(const std::string &format, Args&& ...args)
	{
		m_stream << fmt::format(format, std::forward<Args>(args)...) << "\n";
	}

	template <class ...Args>
	void printLaser(const std::string &format, Args&& ...args)
	{
		print(format, std::forward<Args>(args)...);
	}

	template <class ...Args>
	void printMove(const std::string &format, Args&& ...args)
	{
		print(format, std::forward<Args>(args)..., "F"_a=m_settings.feedRate());
	}

	void laserOn();
	void laserOff();
	void linearMove(const QVector2D &to);
	void fastMove(const QVector2D &to);
	void cwArc(const QVector2D &relativeCenter, const QVector2D &to);
	void ccwArc(const QVector2D &relativeCenter, const QVector2D &to);
};

}

