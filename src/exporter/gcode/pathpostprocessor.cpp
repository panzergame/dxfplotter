#include <pathpostprocessor.h>

namespace Exporter::GCode
{

PathPostProcessor::PathPostProcessor(const Model::PathSettings &settings, const Config::Tools::Tool& tool, std::ostream &stream)
	:PostProcessor(tool, stream),
	m_settings(settings)
{
}

void PathPostProcessor::preCut()
{
	printWithSettings(m_gcode.preCut());
}

void PathPostProcessor::planeLinearMove(const QVector2D &to)
{
	printWithSettings(m_gcode.planeLinearMove(), "X"_a=to.x(), "Y"_a=to.y(), "F"_a=m_settings.planeFeedRate());
}

void PathPostProcessor::depthLinearMove(float depth)
{
	printWithSettings(m_gcode.depthLinearMove(), "Z"_a=depth, "F"_a=m_settings.depthFeedRate());
}

void PathPostProcessor::cwArcMove(const QVector2D &relativeCenter, const QVector2D &to)
{
	printWithSettings(m_gcode.cwArcMove(), "X"_a=to.x(), "Y"_a=to.y(),
		"I"_a=relativeCenter.x(), "J"_a=relativeCenter.y(), "F"_a=m_settings.planeFeedRate());
}

void PathPostProcessor::ccwArcMove(const QVector2D &relativeCenter, const QVector2D &to)
{
	printWithSettings(m_gcode.ccwArcMove(), "X"_a=to.x(), "Y"_a=to.y(),
		"I"_a=relativeCenter.x(), "J"_a=relativeCenter.y(), "F"_a=m_settings.planeFeedRate()); // TODO function for plane move / depth move
}

}
