#include <postprocessor.h>

namespace Exporter::GCode
{

PostProcessor::PostProcessor(const Format &format, std::ostream &stream)
	:m_stream(stream),
	m_format(format)
{
}

void PostProcessor::toolOff()
{
	print(m_format.toolOff);
}

void PostProcessor::fastMove(const QVector2D &to)
{
	print(m_format.fastMove, "X"_a=to.x(), "Y"_a=to.y());
}

}
