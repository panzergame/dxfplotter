#pragma once

#include <model/task.h>

#include <exporter/gcode/format.h>

#include <fstream>

namespace Exporter::GCode
{

class PathPostProcessor;

class Exporter
{
private:
	std::ofstream m_file;
	Format m_format;

	void convertToGCode(const Model::Task *task);
	void convertToGCode(const Model::Path *path);
	void convertToGCode(PathPostProcessor &processor, const Geometry::Polyline &polyline);
	void convertToGCode(PathPostProcessor &processor, const Geometry::Bulge &bulge);

public:
	explicit Exporter(const Model::Task *task, const Format& format, const std::string &filename);
	~Exporter() = default;
};

}

