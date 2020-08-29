#pragma once

#include <yaml-cpp/yaml.h>
#include <config/node.h>

namespace Config
{

class Variable : public Node
{
public:
	/// Type of the variable
	enum class Type
	{
		INT = 0,
		FLOAT,
		STRING
	};

private:
	Type m_type;
	YAML::Node m_property;

public:
	template <class ValueType>
	explicit Variable(const std::string& name, Type type, YAML::Node &section, const ValueType &defaultValue)
		:Node(name),
		m_type(type),
		m_property(section[name])
	{
		// Assigne default value if node isn't defined
		if (!m_property.IsDefined()) {
			m_property = defaultValue;
		}
	}

	Type type() const;

	template <typename ValueType>
	operator ValueType() const
	{
		return m_property.as<ValueType>();
	}

	template <typename ValueType>
	Variable &operator=(const ValueType &value)
	{
		m_property = value;
		return *this;
	}
};

}
