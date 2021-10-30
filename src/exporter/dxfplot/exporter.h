#pragma once

#include <model/document.h>

#include <cereal/archives/json.hpp>

#include <fstream>

namespace exporter::dxfplot
{

class Exporter
{
public:
	explicit Exporter() = default;

	void operator()(const model::Document& document, std::ostream &output)  const;

private:
	using Archive = cereal::JSONOutputArchive;

	void save(Archive &archive, const model::Document& document) const;
};

}

