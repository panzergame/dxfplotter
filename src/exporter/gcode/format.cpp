#include <format.h>

namespace Exporter::GCode
{

Format::Format(const Config::Tools::Tool::Gcode &formatConfig)
	:toolOn(formatConfig.toolOn()),
	toolOff(formatConfig.toolOff()),
	linearMove(formatConfig.linearMove()),
	fastMove(formatConfig.fastMove()),
	cwArc(formatConfig.cwArc()),
	ccwArc(formatConfig.ccwArc())
{
}

}

