#pragma once

namespace Common
{

class FileCouldNotOpenException : public std::exception
{
};

class ImportCouldNotFindToolConfigException : public std::exception
{
};

class ImportCouldNotFindProfileConfigException : public std::exception
{
};

}
