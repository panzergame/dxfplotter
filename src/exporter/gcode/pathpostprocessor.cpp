#include <pathpostprocessor.h>

namespace Exporter::GCode
{

PathPostProcessor::PathPostProcessor(const Model::PathSettings &settings, const Format &format, std::ostream &stream)
	:PostProcessor(format, stream),
	m_settings(settings)
{
}

void PathPostProcessor::toolOn()
{
	printWithSettings(m_format.toolOn);
}

void PathPostProcessor::linearMove(const QVector2D &to)
{
	printWithSettings(m_format.linearMove, "X"_a=to.x(), "Y"_a=to.y());
}

void PathPostProcessor::cwArc(const QVector2D &relativeCenter, const QVector2D &to)
{
	printWithSettings(m_format.cwArc, "X"_a=to.x(), "Y"_a=to.y(),
		"I"_a=relativeCenter.x(), "J"_a=relativeCenter.y());
}

void PathPostProcessor::ccwArc(const QVector2D &relativeCenter, const QVector2D &to)
{
	printWithSettings(m_format.ccwArc, "X"_a=to.x(), "Y"_a=to.y(),
		"I"_a=relativeCenter.x(), "J"_a=relativeCenter.y());
}

}
