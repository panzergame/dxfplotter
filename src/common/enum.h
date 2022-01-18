#pragma once

#include <initializer_list>

namespace common::enumerate
{

template <class EnumType>
inline std::initializer_list<EnumType> All();

template <class EnumType>
inline std::string toString(const EnumType &value);

template <class EnumType>
inline EnumType fromString(const std::string &value);

}
