#pragma once

#include <model/task.h>

#include <sstream>

namespace Exporter::GCode
{

class PathConverter; // TODO

class Exporter
{
private:

	std::stringstream m_fileContent;

	void convertToGCode(const Model::Task &task);
	void convertToGCode(Model::Path *path);
	void convertToGCode(PathConverter &converter, const Geometry::Polyline &polyline);
	void convertToGCode(PathConverter &converter, const Geometry::Bulge &bulge);

public:
	explicit Exporter(const Model::Task &task, const std::string &filename);
};

}

