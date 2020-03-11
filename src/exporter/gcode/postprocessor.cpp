#include <postprocessor.h>

namespace Exporter::GCode
{

PostProcessor::PostProcessor(const Model::Path::Settings &settings, std::stringstream &stream)
	:m_settings(settings),
	m_stream(stream)
{
}

void PostProcessor::laserOn()
{
	printLaser("M1 {S}", "S"_a=m_settings.intensity());
}

void PostProcessor::laserOff()
{
	printLaser("M2");
}

void PostProcessor::linearMove(const QVector2D &to)
{
	printMove("G1 X {X} Y {Y} F {F}", "X"_a=to.x(), "Y"_a=to.y());
}

void PostProcessor::fastMove(const QVector2D &to)
{
	printMove("G0 X {X} Y {Y}", "X"_a=to.x(), "Y"_a=to.y());
}

void PostProcessor::cwArc(const QVector2D &relativeCenter, const QVector2D &to)
{
	printMove("G2 X {X} Y {Y} I {I} J {J} F {F}", "X"_a=to.x(), "Y"_a=to.y(),
		"I"_a=relativeCenter.x(), "J"_a=relativeCenter.y());
}

void PostProcessor::ccwArc(const QVector2D &relativeCenter, const QVector2D &to)
{
	printMove("G3 X {X} Y {Y} I {I} J {J} F {F}", "X"_a=to.x(), "Y"_a=to.y(),
		"I"_a=relativeCenter.x(), "J"_a=relativeCenter.y());
}

}
