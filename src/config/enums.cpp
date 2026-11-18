module;

#include <initializer_list>
#include <string>
#include <unordered_map>

export module config.enums;
import common.enumerate;

export namespace config
{

enum class ToolType { Rotary, Laser, DragKnife };

}

namespace common::enumerate
{

template<>
inline std::initializer_list<config::ToolType> All()
{
	static const std::initializer_list<config::ToolType> values
		= { config::ToolType::Rotary, config::ToolType::Laser, config::ToolType::DragKnife };
	return values;
}

template<>
inline std::string toString(const config::ToolType& direction)
{
	static const std::string translations[] = {
		"rotary", // Rotary
		"laser", // Laser
		"drag knife" // DragKnife
	};

	return translations[static_cast<int>(direction)];
}

template<>
inline config::ToolType fromString(const std::string& value)
{
	static const std::unordered_map<std::string, config::ToolType> translations
		= { { "rotary", config::ToolType::Rotary },
			{ "laser", config::ToolType::Laser },
			{ "drag knife", config::ToolType::DragKnife } };

	return translations.at(value);
}

}