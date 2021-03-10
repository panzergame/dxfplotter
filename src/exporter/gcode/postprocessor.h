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
	std::ostream &m_stream;

protected:
	const Config::Tools::Tool& m_tool;
	const Config::Profiles::Profile::Gcode& m_gcode;

	// TODO manage exception
	/** Print a command to stream with a format and a list of named arguments
	 * @param format A fmt valid format string.
	 * @param args List of named arguments
	 */
	template <class ...Args>
	void print(const std::string &format, Args&& ...args)
	{
		m_stream << fmt::format(format, std::forward<Args>(args)...) << "\n";
	}

public:
	explicit PostProcessor(const Config::Tools::Tool& tool, const Config::Profiles::Profile::Gcode& gcode, std::ostream &stream);

	void postCut();
	void fastPlaneMove(const QVector2D &to);
	void retractDepth();
};

}

