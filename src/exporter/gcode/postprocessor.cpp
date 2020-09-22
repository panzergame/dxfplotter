#include <postprocessor.h>

namespace Exporter::GCode
{

PostProcessor::PostProcessor(const Config::Tools::Tool& tool, std::ostream &stream)
	:m_stream(stream),
	m_tool(tool),
	m_gcode(m_tool.gcode())
{
}

void PostProcessor::postCut()
{
	print(m_gcode.postCut());
}

void PostProcessor::fastPlaneMove(const QVector2D &to)
{
	print(m_gcode.planeFastMove(), "X"_a=to.x(), "Y"_a=to.y());
}

void PostProcessor::retractDepth()
{
	print(m_gcode.depthFastMove(), "Z"_a=m_tool.general().retractDepth());
}

}
