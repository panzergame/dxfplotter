#pragma once

#include <cereal/cereal.hpp>
#include <model/document.h>

#include <iostream> // TODO

namespace cereal
{

template <class Archive>
void load(Archive &archive, Model::Document &document)
{
}

template <> struct LoadAndConstruct<Model::Document>
{
	template <class Archive>
	static void load_and_construct(Archive &archive, cereal::construct<Model::Document> &construct)
	{
		//archive(cereal::make_nvp("task", document.task()));
		std::string profileName;
		archive(cereal::make_nvp("profile_name", profileName));
		std::string toolName;
		archive(cereal::make_nvp("tool_name", toolName));
		
		
		std::cout << profileName << " " << toolName << std::endl;
		const Config::Tools::Tool tool = archive.m_tools[toolName];
		const Config::Profiles::Profile profile = archive.m_profiles[toolName];
		construct(nullptr, tool, profile);
	}
};

}
