module;

#include <QVector2D>

#include <sstream>
#include <string>
#include <fmt/format.h>

export module exporter.gcode.postprocessor;

import common.exception;
import exporter.gcode.exporter;
import geometry.bulge;
import geometry.circle;
import geometry.line;
import geometry.utils;
import model.path;
import config.config;

export namespace exporter::gcode
{

class PostProcessor
{
private:
	std::ostream& m_stream;

protected:
	const config::Profiles::Profile::Gcode& m_gcode;

	/** Print a command to stream with a format and a list of named arguments
	 * @param format A fmt valid format string.
	 * @param args List of named arguments
	 */
	template<class... Args>
	void print(const std::string& format, Args&&... args)
	{
		try {
			m_stream << fmt::format(fmt::runtime(format), std::forward<Args>(args)...) << "\n";
		} catch (const fmt::format_error& exception) {
			throw common::GCodeFormatException(format, exception.what(), fmt::to_string(args.name)...);
		}
	}

public:
	void preCut(float intensity);
	void postCut();
	void planeLinearMove(const QVector2D& to, float feedRate);
	void depthLinearMove(float depth, float feedRate);
	void cwArcMove(const QVector2D& relativeCenter, const QVector2D& to, float feedRate);
	void ccwArcMove(const QVector2D& relativeCenter, const QVector2D& to, float feedRate);
	void fastPlaneMove(const QVector2D& to);
	void retractDepth(float depth);
	void processBulge(const geometry::Bulge& bulge, float planeFeedRate);
	void processLine(const geometry::Bulge& bulge, float planeFeedRate);
	void processArc(const geometry::Bulge& bulge, float planeFeedRate);

	explicit PostProcessor(const config::Profiles::Profile::Gcode& gcode, std::ostream& stream);
};

}

using namespace fmt::literals;

namespace exporter::gcode
{

PostProcessor::PostProcessor(const config::Profiles::Profile::Gcode& gcode, std::ostream& stream)
	: m_stream(stream)
	, m_gcode(gcode)
{
}

void PostProcessor::preCut(float intensity)
{
	print(m_gcode.preCut(), "S"_a = intensity);
}

void PostProcessor::postCut()
{
	print(m_gcode.postCut());
}

void PostProcessor::planeLinearMove(const QVector2D& to, float feedRate)
{
	print(m_gcode.planeLinearMove(), "X"_a = to.x(), "Y"_a = to.y(), "F"_a = feedRate);
}

void PostProcessor::depthLinearMove(float depth, float feedRate)
{
	print(m_gcode.depthLinearMove(), "Z"_a = depth, "F"_a = feedRate);
}

void PostProcessor::cwArcMove(const QVector2D& relativeCenter, const QVector2D& to, float feedRate)
{
	print(m_gcode.cwArcMove(), "X"_a = to.x(), "Y"_a = to.y(), "I"_a = relativeCenter.x(), "J"_a = relativeCenter.y(),
		  "F"_a = feedRate);
}

void PostProcessor::ccwArcMove(const QVector2D& relativeCenter, const QVector2D& to, float feedRate)
{
	print(m_gcode.ccwArcMove(), "X"_a = to.x(), "Y"_a = to.y(), "I"_a = relativeCenter.x(), "J"_a = relativeCenter.y(),
		  "F"_a = feedRate);
}

void PostProcessor::fastPlaneMove(const QVector2D& to)
{
	print(m_gcode.planeFastMove(), "X"_a = to.x(), "Y"_a = to.y());
}

void PostProcessor::retractDepth(float depth)
{
	print(m_gcode.depthFastMove(), "Z"_a = depth);
}

void PostProcessor::processBulge(const geometry::Bulge& bulge, float planeFeedRate)
{
	if (bulge.isArc()) {
		processArc(bulge, planeFeedRate);
	} else {
		processLine(bulge, planeFeedRate);
	}
}

void PostProcessor::processLine(const geometry::Bulge& bulge, float planeFeedRate)
{
	planeLinearMove(bulge.end(), planeFeedRate);
}

void PostProcessor::processArc(const geometry::Bulge& bulge, float planeFeedRate)
{
	const geometry::Circle circle = bulge.toCircle();
	// Relative center to start
	const QVector2D relativeCenter = circle.center() - bulge.start();
	switch (circle.orientation()) {
		case geometry::Orientation::CW:
			cwArcMove(relativeCenter, bulge.end(), planeFeedRate);
			break;
		case geometry::Orientation::CCW:
			ccwArcMove(relativeCenter, bulge.end(), planeFeedRate);
			break;
		default:
			break;
	}
}

}
