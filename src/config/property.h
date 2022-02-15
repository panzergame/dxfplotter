#pragma once

#include <config/node.h>

#include <config/utils.h>

namespace config
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
			*this = defaultValue;
		}
	}

	Property() = default;

	operator ValueType() const
	{
		return fromSerializable<ValueType>(m_yamlNode);
	}

	Property &operator=(const ValueType &value)
	{
		m_yamlNode = toSerializable<ValueType>(value);
		return *this;
	}
};

}
