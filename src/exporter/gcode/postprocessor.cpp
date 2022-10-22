#include <postprocessor.h>

using namespace fmt::literals;

namespace exporter::gcode
{

PostProcessor::PostProcessor(const config::Profiles::Profile::Gcode& gcode, std::ostream &stream)
	:m_stream(stream),
	m_gcode(gcode)
{
}

void PostProcessor::preCut(float intensity)
{
	print(m_gcode.preCut(), "S"_a=intensity);
}

void PostProcessor::postCut()
{
	print(m_gcode.postCut());
}

void PostProcessor::planeLinearMove(const QVector2D &to, float feedRate, float intensity)
{
	print(m_gcode.planeLinearMove(), "X"_a=to.x(), "Y"_a=to.y(), "F"_a=feedRate, "S"_a=intensity);
}

void PostProcessor::depthLinearMove(float depth, float feedRate, float intensity)
{
	print(m_gcode.depthLinearMove(), "Z"_a=depth, "F"_a=feedRate, "S"_a=intensity);
}

void PostProcessor::cwArcMove(const QVector2D &relativeCenter, const QVector2D &to, float feedRate, float intensity)
{
	print(m_gcode.cwArcMove(), "X"_a=to.x(), "Y"_a=to.y(),
		"I"_a=relativeCenter.x(), "J"_a=relativeCenter.y(), "F"_a=feedRate, "S"_a=intensity);
}

void PostProcessor::ccwArcMove(const QVector2D &relativeCenter, const QVector2D &to, float feedRate, float intensity)
{
	print(m_gcode.ccwArcMove(), "X"_a=to.x(), "Y"_a=to.y(),
		"I"_a=relativeCenter.x(), "J"_a=relativeCenter.y(), "F"_a=feedRate, "S"_a=intensity);
}

void PostProcessor::fastPlaneMove(const QVector2D &to)
{
	print(m_gcode.planeFastMove(), "X"_a=to.x(), "Y"_a=to.y());
}

void PostProcessor::retractDepth(float depth)
{
	print(m_gcode.depthFastMove(), "Z"_a=depth);
}

}
