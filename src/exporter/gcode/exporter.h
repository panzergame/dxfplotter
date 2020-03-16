#pragma once

#include <model/task.h>

#include <exporter/gcode/format.h>

#include <sstream>

namespace Exporter::GCode
{

class PostProcessor;

class Exporter
{
private:
	std::stringstream m_fileContent;
	Format m_format;

	void convertToGCode(const Model::Task *task);
	void convertToGCode(const Model::Path *path);
	void convertToGCode(PostProcessor &processor, const Geometry::Polyline &polyline);
	void convertToGCode(PostProcessor &processor, const Geometry::Bulge &bulge);

public:
	explicit Exporter(const Model::Task *task, const Format& format, const std::string &filename);
};

}

