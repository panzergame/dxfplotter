#include <postprocessor.h>

namespace Exporter::GCode
{

PostProcessor::PostProcessor(const Model::PathSettings &settings, const Format &format, std::ostream &stream)
	:m_settings(settings),
	m_format(format),
	m_stream(stream)
{
}

void PostProcessor::toolOn()
{
	printTool(m_format.toolOn, "S"_a=m_settings.intensity());
}

void PostProcessor::toolOff()
{
	printTool(m_format.toolOff);
}

void PostProcessor::linearMove(const QVector2D &to)
{
	printMove(m_format.linearMove, "X"_a=to.x(), "Y"_a=to.y());
}

void PostProcessor::fastMove(const QVector2D &to)
{
	printMove(m_format.fastMove, "X"_a=to.x(), "Y"_a=to.y());
}

void PostProcessor::cwArc(const QVector2D &relativeCenter, const QVector2D &to)
{
	printMove(m_format.cwArc, "X"_a=to.x(), "Y"_a=to.y(),
		"I"_a=relativeCenter.x(), "J"_a=relativeCenter.y());
}

void PostProcessor::ccwArc(const QVector2D &relativeCenter, const QVector2D &to)
{
	printMove(m_format.ccwArc, "X"_a=to.x(), "Y"_a=to.y(),
		"I"_a=relativeCenter.x(), "J"_a=relativeCenter.y());
}

}