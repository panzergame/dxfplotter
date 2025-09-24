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
	const config::Profiles::Profile::Gcode& m_gcode;

	/** Print a command to stream with a format and a list of named arguments
	 * @param format A fmt valid format string.
	 * @param args List of named arguments
	 */
	template <class ...Args>
	void print(const std::string &format, Args&& ...args)
	{
		try {
			m_stream << fmt::format(fmt::runtime(format), std::forward<Args>(args)...) << "\n";
		}
		catch (const fmt::format_error &exception) {
			throw common::GCodeFormatException(format, exception.what(), fmt::to_string(args.name)...);
		}
	}

public:
	void preCut(float intensity);
	void postCut();
	void planeLinearMove(const QVector2D &to, float feedRate);
	void depthLinearMove(float depth, float feedRate);
	void cwArcMove(const QVector2D &relativeCenter, const QVector2D &to, float feedRate);
	void ccwArcMove(const QVector2D &relativeCenter, const QVector2D &to, float feedRate);
	void fastPlaneMove(const QVector2D &to);
	void retractDepth(float depth);
	void processBulge(const geometry::Bulge &bulge, float planeFeedRate);
	void processLine(const geometry::Bulge &bulge, float planeFeedRate);
	void processArc(const geometry::Bulge &bulge, float planeFeedRate);

	explicit PostProcessor(const config::Profiles::Profile::Gcode& gcode, std::ostream &stream);
};

}

