#include <format.h>

namespace Exporter::GCode
{

Format::Format(const Config::Section &formatConfig)
	:toolOn(formatConfig.var("tool_on")),
	toolOff(formatConfig.var("tool_off")),
	linearMove(formatConfig.var("linear_move")),
	fastMove(formatConfig.var("fast_move")),
	cwArc(formatConfig.var("cw_arc")),
	ccwArc(formatConfig.var("ccw_arc"))
{
}

}

