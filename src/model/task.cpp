#include <task.h>

namespace Model
{

Task::Task(Paths &paths)
{
	m_stack.resize(paths.size());
	std::transform(paths.begin(), paths.end(), m_stack.begin(),
		[](Path &path){ return &path; });
}

int Task::count() const
{
	return m_stack.size();
}

const PathsPtr &Task::stack() const
{
	return m_stack;
}

}
