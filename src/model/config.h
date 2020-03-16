#pragma once

#include <model/configvariable.h>

namespace Model
{

class Config
{
public:
	class Dxf
	{
	public:
		ConfigVariable<float> splinePrecision;
		ConfigVariable<float> assembleTolerance;

		explicit Dxf(INI::Section *section);
	};

	class GcodeFormat
	{
	public:
		ConfigVariable<std::string> toolOn;
		ConfigVariable<std::string> toolOff;
		ConfigVariable<std::string> linearMove;
		ConfigVariable<std::string> fastMove;
		ConfigVariable<std::string> cwArc;
		ConfigVariable<std::string> ccwArc;

		explicit GcodeFormat(INI::Section *section);
	};

private:
	INI::File m_file;
	Dxf m_dxf;
	GcodeFormat m_gcodeFormat;

public:
	explicit Config();
	~Config();

	/// Dxf configuration variables
	Dxf &dxf();
	/// Gcode format configuration variables
	GcodeFormat &gcodeFormat();
};

}
