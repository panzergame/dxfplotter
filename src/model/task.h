#pragma once

#include <model/path.h>

namespace Model
{

class Task
{
private:
	PathsPtr m_stack;

public:
	Task() = default;
	explicit Task(Paths &paths);

	int count() const;
	const PathsPtr &stack() const;
};

}

