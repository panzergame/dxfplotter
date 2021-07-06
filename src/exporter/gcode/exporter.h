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

	void convertToGCode(const Model::Task &task, std::ostream &output) const;
	void convertToGCode(const Model::Path &path, std::ostream &output) const;
	void convertToGCode(PathPostProcessor &processor, const Geometry::Polyline &polyline) const;
	void convertToGCode(PathPostProcessor &processor, const Geometry::Polyline &polyline, float maxDepth, Geometry::CuttingDirection cuttingDirection) const;
	void convertToGCode(PathPostProcessor &processor, const Geometry::Bulge &bulge) const;

public:
	explicit Exporter(const Config::Tools::Tool& tool, const Config::Profiles::Profile::Gcode& gcode);
	~Exporter() = default;

	void operator()(const Model::Document& document, std::ostream &output)  const;
};

}

