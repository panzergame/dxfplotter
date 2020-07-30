#pragma once

#include <leksysini/iniparser.hpp>
#include <config/node.h>

namespace Config
{

class Variable : public Node
{
public:
	/// Type of the variable
	enum class Type
	{
		INT,
		FLOAT,
		STRING
	};

private:
	Type m_type;
	INI::Section *m_section;

public:
	template <class Type>
	void init(const Type &defaultValue)
	{
		/*if (!m_section->ContainsKey(m_name)) {
			m_section->SetValue(m_name, defaultValue); // TODO comment
		}*/
	}

	explicit Variable(const std::string& name, Type type)
		:Node(name),
		m_type(type),
		m_section(nullptr) // TODO
	{
	}

	template <typename Type>
	operator Type() const
	{
		return m_section->GetValue(m_name).template Get<Type>();
	}

	template <typename Type>
	Variable &operator=(const Type &value)
	{
		m_section->SetValue(m_name, value);
		return *this;
	}
};

}
