#pragma once

#include <model/document.h>

#include <config/config.h>

#include <fstream>

namespace exporter::gcode
{

class PathPostProcessor;

class Exporter
{
public:
	enum Options
	{
		None = 0,
		ExportConfig = (1 << 0)
	};

private:
	const config::Tools::Tool &m_tool;
	const config::Profiles::Profile &m_profile;
	const Options m_options;

	void convertToGCode(const model::Task &task, std::ostream &output) const;
	void convertToGCode(const model::Path &path, std::ostream &output) const;
	void convertToGCode(PathPostProcessor &processor, const geometry::Polyline &polyline) const;
	void convertToGCode(PathPostProcessor &processor, const geometry::Polyline &polyline, float maxDepth, geometry::CuttingDirection cuttingDirection) const;
	void convertToGCode(PathPostProcessor &processor, const geometry::Bulge &bulge) const;

public:
	explicit Exporter(const config::Tools::Tool& tool, const config::Profiles::Profile& profile, Options options = None);
	~Exporter() = default;

	void operator()(const model::Document& document, std::ostream &output)  const;
};

}

