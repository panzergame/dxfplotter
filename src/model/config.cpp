#include <config.h>

#include <iostream> // TODO

#include <climits>

namespace Model
{

Config::Dxf::Dxf(INI::Section *section)
	:splinePrecision("spline_precision", 0.001, section),
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
}

Config::Config()
	:m_file("settings.ini"),
	m_dxf(m_file.GetSection("Dxf")),
	m_gcodeFormat(m_file.GetSection("Gcode"))
{
}

Config::~Config()
{
	m_file.Save("settings.ini"); // TODO path
}

Config::Dxf &Config::dxf()
{
	return m_dxf;
}

Config::GcodeFormat &Config::gcodeFormat()
{
	return m_gcodeFormat;
}

}
