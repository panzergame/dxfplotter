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
		ExportConfig = (1 << 0),
		ExportMetadata = (1 << 1)
	};

private:
	const config::Tools::Tool &m_tool;
	const config::Profiles::Profile &m_profile;
	const Options m_options;

public:
	explicit Exporter(const config::Tools::Tool& tool, const config::Profiles::Profile& profile, Options options = None);
	~Exporter() = default;

	void operator()(const model::Document& document, std::ostream &output)  const;
};

}

