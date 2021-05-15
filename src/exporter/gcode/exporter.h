#pragma once

#include <model/document.h>

#include <config/config.h>

#include <fstream>

namespace Exporter::GCode
{

class PathPostProcessor;

class Exporter
{
private:  
	const Config::Tools::Tool &m_tool;
	const Config::Profiles::Profile::Gcode &m_gcode;

	void convertToGCode(const Model::Task &task, std::ostream &output);
	void convertToGCode(const Model::Path &path, std::ostream &output);
	void convertToGCode(PathPostProcessor &processor, const Geometry::Polyline &polyline);
	void convertToGCode(PathPostProcessor &processor, const Geometry::Polyline &polyline, float maxDepth);
	void convertToGCode(PathPostProcessor &processor, const Geometry::Bulge &bulge);

public:
	explicit Exporter(const Config::Tools::Tool& tool, const Config::Profiles::Profile::Gcode& gcode);
	~Exporter() = default;

	void operator()(const Model::Document& document, std::ostream &output);
};

}

