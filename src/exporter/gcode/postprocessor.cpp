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
	printLaser("M1");
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
	printMove("G0 X {X} Y {Y} F {F}", "X"_a=to.x(), "Y"_a=to.y());
}

void PostProcessor::cwArc()
{
	
}

void PostProcessor::ccwArc()
{
	
}

}
