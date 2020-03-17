#pragma once

#include <leksysini/iniparser.hpp>
#include <string>

namespace Model
{

template <class Type>
class ConfigVariable
{
private:
	const std::string m_name;
	INI::Section *m_section;

	void initDefaultValue(const Type &defaultValue)
	{
		if (!m_section->ContainsKey(m_name)) {
			m_section->SetValue(m_name, defaultValue); // TODO comment
		}
	}

public:
	explicit ConfigVariable(const std::string& name, const Type &defaultValue, INI::Section *section)
		:m_name(name),
		m_section(section)
	{
		initDefaultValue(defaultValue);
	}

	operator Type() const
	{
		return m_section->GetValue(m_name).template Get<Type>();
	}

	ConfigVariable<Type> &operator=(const Type &value)
	{
		m_section->SetValue(m_name, value);
		return *this;
	}
};

}
