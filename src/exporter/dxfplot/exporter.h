#pragma once

#include <model/document.h>

#include <cereal/archives/json.hpp>

#include <fstream>

namespace Exporter::Dxfplot
{

class Exporter
{
public:
	explicit Exporter() = default;

	void operator()(const Model::Document& document, std::ostream &output)  const;

private:
	using Archive = cereal::JSONOutputArchive;

	void save(Archive &archive, const Model::Document& document) const;
};

}

