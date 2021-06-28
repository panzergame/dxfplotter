#pragma once

#include <model/document.h>

#include <config/config.h>

#include <fstream>

namespace Exporter::Dxfplot
{

class Exporter
{
public:
	explicit Exporter() = default;

	void operator()(const Model::Document::UPtr& document, std::ostream &output)  const;
};

}

