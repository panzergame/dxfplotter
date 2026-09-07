module;

#include <string>
#include <type_traits>
#include <yaml-cpp/yaml.h>

export module config.utils;

import common.enumerate;
import geometry.utils;

import common.enumerate;
import geometry.utils;

export namespace config
{

template<class Type>
inline std::enable_if_t<!std::is_enum_v<Type>, Type> toSerializable(const Type& value)
{
	return value;
}

template<class EnumType>
inline std::enable_if_t<std::is_enum_v<EnumType>, std::string> toSerializable(const EnumType& value)
{
	return common::enumerate::toString(value);
}

template<class Type>
inline Type fromSerializable(const YAML::Node& node)
{
	if constexpr (std::is_enum_v<Type>) {
		return common::enumerate::fromString<Type>(node.as<std::string>());
	} else {
		return node.as<Type>();
	}
}

}
