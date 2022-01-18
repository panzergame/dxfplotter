#pragma once

#include <yaml-cpp/yaml.h>

#include <geometry/utils.h>

namespace config
{

template <class Type>
inline std::enable_if_t<!std::is_enum<Type>::value, Type> toSerializable(const Type &value)
{
	return value;
}

template <class EnumType>
inline std::enable_if_t<std::is_enum<EnumType>::value, std::string> toSerializable(const EnumType& value)
{
	return common::enumerate::toString(value);
}

template <class Type>
inline std::enable_if_t<!std::is_enum<Type>::value, Type> fromSerializable(const YAML::Node &node)
{
	return node.as<Type>();
}

template <class EnumType>
inline std::enable_if_t<std::is_enum<EnumType>::value, EnumType> fromSerializable(const YAML::Node &node)
{
	return common::enumerate::fromString<EnumType>(node.as<std::string>());
}

}
