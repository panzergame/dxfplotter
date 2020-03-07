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

Config::Config()
	:m_file("settings.ini"),
	m_dxf(m_file.GetSection("Dxf"))
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

}
