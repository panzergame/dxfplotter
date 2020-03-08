#include <task.h>

namespace Model
{

Task::Task(Path::List &paths)
{
	m_stack.resize(paths.size());
	std::transform(paths.begin(), paths.end(), m_stack.begin(),
		[](Path &path){ return &path; });
}

int Task::count() const
{
	return m_stack.size();
}

Path *Task::pathAt(int index) const
{
	assert(0 <= index && index < count());
	return m_stack[index];
}

}
