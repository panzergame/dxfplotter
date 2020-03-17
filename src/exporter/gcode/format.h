#pragma once

#include <model/config.h> // TODO move config in model/config/

namespace Exporter::GCode
{

class Format
{
public:
	explicit Format(const Model::Config::GcodeFormat &format);

	std::string toolOn;
	std::string toolOff;
	std::string linearMove;
	std::string fastMove;
	std::string cwArc;
	std::string ccwArc;
};

}
