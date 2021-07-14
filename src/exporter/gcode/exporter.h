#pragma once

#include <model/task.h>

#include <config/config.h>

#include <fstream>

namespace Exporter::GCode
{

class PathPostProcessor;

class Exporter
{
private:
	std::ostream &m_output;
  
	const Config::Tools::Tool &m_tool;
	const Config::Profiles::Profile::Gcode &m_gcode;

	void convertToGCode(const Model::Task &task);
	void convertToGCode(const Model::Path &path);
	void convertToGCode(PathPostProcessor &processor, const Geometry::Polyline &polyline);
	void convertToGCode(PathPostProcessor &processor, const Geometry::Polyline &polyline, float maxDepth, Geometry::CuttingDirection cuttingDirection);
	void convertToGCode(PathPostProcessor &processor, const Geometry::Bulge &bulge);

public:
	explicit Exporter(const Model::Task &task, const Config::Tools::Tool& tool, const Config::Profiles::Profile::Gcode& gcode, std::ostream &output);
	~Exporter() = default;
};

}

