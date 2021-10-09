#pragma once

#include <sstream>
#include <string>

namespace Common
{

class FileCouldNotOpenException : public std::exception
{
};

class GCodeFormatException : public std::runtime_error
{
private:
	template <class ... Args>
	std::string errorMessage(const std::string &format, const char *error, Args&& ... args)
	{
		std::ostringstream stream;
		stream << "Formatting error, format \"" << format << "\" with arguments";

		((stream << ' ' << args), ...);
		stream << ": " << error;

		return stream.str();
	}

public:
	template <class ... Args>
	explicit GCodeFormatException(const std::string &format, const char *error, Args&& ... args)
		:std::runtime_error(errorMessage(format, error, std::forward<Args>(args)...))
	{
	}
};

class ImportCouldNotFindToolConfigException : public std::exception
{
};

class ImportCouldNotFindProfileConfigException : public std::exception
{
};

}
