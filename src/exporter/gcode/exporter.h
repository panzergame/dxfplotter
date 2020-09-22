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
	std::ofstream m_file;
  
	const Config::Tools::Tool &m_tool;

	void convertToGCode(const Model::Task *task);
	void convertToGCode(const Model::Path *path);
	void convertToGCode(PathPostProcessor &processor, const Geometry::Polyline &polyline);
	void convertToGCode(PathPostProcessor &processor, const Geometry::Polyline &polyline, float maxDepth);
	void convertToGCode(PathPostProcessor &processor, const Geometry::Bulge &bulge);

public:
	explicit Exporter(const Model::Task *task, const Config::Tools::Tool& tool, const std::string &filename);
	~Exporter() = default;
};

}

