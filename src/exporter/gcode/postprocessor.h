#pragma once

#include <model/path.h>
#include <exporter/gcode/exporter.h>
#include <common/exception.h>

#include <fmt/format.h>
#include <sstream>

namespace exporter::gcode
{

class PostProcessor
{
private:
	std::ostream &m_stream;

protected:
	const config::Tools::Tool& m_tool;
	const config::Profiles::Profile::Gcode& m_gcode;

	/** Print a command to stream with a format and a list of named arguments
	 * @param format A fmt valid format string.
	 * @param args List of named arguments
	 */
	template <class ...Args>
	void print(const std::string &format, Args&& ...args)
	{
		try {
			m_stream << fmt::format(format, std::forward<Args>(args)...) << "\n";
		}
		catch (const fmt::format_error &exception) {
			throw Common::GCodeFormatException(format, exception.what(), fmt::to_string(args.name)...);
		}
	}

public:
	explicit PostProcessor(const config::Tools::Tool& tool, const config::Profiles::Profile::Gcode& gcode, std::ostream &stream);

	void postCut();
	void fastPlaneMove(const QVector2D &to);
	void retractDepth();
};

}

