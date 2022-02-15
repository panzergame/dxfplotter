#pragma once

#include <yaml-cpp/yaml.h>

#include <geometry/utils.h>

namespace config
{

template <class Type>
inline std::enable_if_t<!std::is_enum_v<Type>, Type> toSerializable(const Type &value)
{
	return value;
}

template <class EnumType>
inline std::enable_if_t<std::is_enum_v<EnumType>, std::string> toSerializable(const EnumType& value)
{
	return common::enumerate::toString(value);
}

template <class Type>
inline Type fromSerializable(const YAML::Node &node)
{
	if constexpr(std::is_enum_v<Type>) {
		return common::enumerate::fromString<Type>(node.as<std::string>());
	}
	else {
		return node.as<Type>();
	}
}

}
