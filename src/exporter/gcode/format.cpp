#include <format.h>

namespace Exporter::GCode
{

Format::Format(const Config::Section &formatConfig)
	:toolOn(formatConfig["tool_on"]),
	toolOff(formatConfig["tool_off"]),
	linearMove(formatConfig["linear_move"]),
	fastMove(formatConfig["fast_move"]),
	cwArc(formatConfig["cw_arc"]),
	ccwArc(formatConfig["ccw_arc"])
{
}

}

