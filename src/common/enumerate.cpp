module;

#include <initializer_list>
#include <string>

// Named common.enumerate and not common.enum: 'enum' is a keyword and cannot be a module name component.
export module common.enumerate;

export namespace common::enumerate
{

template<class EnumType>
std::initializer_list<EnumType> All();

template<class EnumType>
std::string toString(const EnumType& value);

template<class EnumType>
EnumType fromString(const std::string& value);

}
