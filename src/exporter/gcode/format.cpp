#include <format.h>

namespace Exporter::GCode
{

Format::Format(const Config::Config::GcodeFormat &format)
	:toolOn(format.toolOn),
	toolOff(format.toolOff),
	linearMove(format.linearMove),
	fastMove(format.fastMove),
	cwArc(format.cwArc),
	ccwArc(format.ccwArc)
{
}

}

