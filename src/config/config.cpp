#include <config.h>

namespace Config
{

/*Config::Dxf::Dxf(INI::Section *section)
	:splineToArcPrecision("spline_to_arc_precision", 0.001, section),
	assembleTolerance("assemble_tolerance", 0.001, section)
{
}

Config::GcodeFormat::GcodeFormat(INI::Section *section)
	:toolOn("tool_on", "M1 {S}", section),
	toolOff("tool_off", "M2", section),
	linearMove("linear_move", "G1 X {X} Y {Y} F {F}", section),
	fastMove("fast_move", "G0 X {X} Y {Y}", section),
	cwArc("cw_arc", "G2 X {X} Y {Y} I {I} J {J} F {F}", section),
	ccwArc("ccw_arc", "G3 X {X} Y {Y} I {I} J {J} F {F}", section)
{
}*/

Config::~Config()
{
	m_file.Save(m_filePath);
}

}
