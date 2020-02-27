#pragma once

#include <model/path.h>

namespace Model
{

class Task
{
private:
	Path::ListPtr m_stack;

public:
	Task() = default;
	explicit Task(Path::List &paths);

	int count() const;
	const Path::ListPtr &stack() const;
};

}

