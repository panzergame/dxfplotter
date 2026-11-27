module;

#include <optional>
#include <string>
#include <yaml-cpp/yaml.h>

export module config.property;

import config.node;
import config.utils;

export namespace config
{

template<class ValueType>
class Property : public Node
{
private:
	YAML::Node m_yamlNode;
	std::optional<ValueType> m_minimum;

public:
	explicit Property(const std::string& name, const std::string& description, const ValueType& defaultValue,
					  YAML::Node yamlNode, std::optional<ValueType> minimum = std::nullopt)
		: Node(name, description)
		, m_yamlNode(yamlNode)
		, m_minimum(minimum)
	{
		// Assign default value if node isn't defined
		if (!m_yamlNode.IsDefined()) {
			*this = defaultValue;
		}
	}

	Property() = default;

	const std::optional<ValueType>& minimum() const { return m_minimum; }

	operator ValueType() const { return fromSerializable<ValueType>(m_yamlNode); }

	Property& operator=(const ValueType& value)
	{
		m_yamlNode = toSerializable<ValueType>(value);
		return *this;
	}
};

}
