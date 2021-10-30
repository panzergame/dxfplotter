#include <pathpostprocessor.h>

using namespace fmt::literals;

namespace exporter::gcode
{

PathPostProcessor::PathPostProcessor(const model::PathSettings &settings, const config::Tools::Tool& tool, const config::Profiles::Profile::Gcode& gcode, std::ostream &stream)
	:PostProcessor(tool, gcode, stream),
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
		"I"_a=relativeCenter.x(), "J"_a=relativeCenter.y(), "F"_a=m_settings.planeFeedRate());
}

}
