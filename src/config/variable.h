#pragma once

#include <leksysini/iniparser.hpp>
#include <config/item.h>

#include <QDebug> // TODO

namespace Config
{

template <class Type>
class Variable : public Item
{
private:
	INI::Section *m_section;

	void initDefaultValue(const Type &defaultValue)
	{
		if (!m_section->ContainsKey(m_name)) {
			m_section->SetValue(m_name, defaultValue); // TODO comment
		}
	}

public:
	explicit Variable(const std::string& name, const std::string &description, const Type &defaultValue, INI::Section *section)
		:Item(name, description),
		m_section(section)
	{
		initDefaultValue(defaultValue);
	}

	operator Type() const
	{
		return m_section->GetValue(m_name).template Get<Type>();
	}

	Variable<Type> &operator=(const Type &value)
	{
		m_section->SetValue(m_name, value);
		return *this;
	}
};

}
