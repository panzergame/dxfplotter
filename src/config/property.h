#pragma once

#include <config/node.h>

namespace Config
{

template <class ValueType>
class Property : public Node
{
private:
	YAML::Node m_yamlNode;

public:
	explicit Property(const std::string& name, const std::string &description, const ValueType &defaultValue, YAML::Node yamlNode)
		:Node(name, description),
		m_yamlNode(yamlNode)
	{
		// Assign default value if node isn't defined
		if (!m_yamlNode.IsDefined()) {
			m_yamlNode = defaultValue;
		}
	}

	Property() = default;

	operator ValueType() const
	{
		return m_yamlNode.as<ValueType>();
	}

	Property &operator=(const ValueType &value)
	{
		m_yamlNode = value;
		return *this;
	}
};

}
