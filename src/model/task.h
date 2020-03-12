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
	explicit Task(const Path::ListPtr &paths);

	int count() const;
	Path *pathAt(int index) const;

	template <class Functor>
	void forEachPath(Functor &&functor) const
	{
		for (Path *path : m_stack) {
			functor(path);
		}
	}
};

}

