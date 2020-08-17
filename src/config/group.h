#pragma once

#include <variant>
#include <vector>
#include <map>

#include <tinyxml2.h>

#include <config/section.h>

namespace Config
{

/** @brief A configuration group.
 * Group contains either sections or groups.
 */
class Group : public NodeList
{
public:
	explicit Group() = default;
	explicit Group(tinyxml2::XMLElement *root, YAML::Node &section);

	template <class Index>
	Section &section(const Index &id)
	{
		return at<Section>(id);
	}

	template <class Index>
	const Section &section(const Index &id) const
	{
		return at<Section>(id);
	}

	template <class Index>
	Group &group(const Index &id)
	{
		return at<Group>(id);
	}

	template <class Index>
	const Group &group(const Index &id) const
	{
		return at<Group>(id);
	}
};

}
