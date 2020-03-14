#include <task.h>

namespace Model
{

Task::Task(const Path::ListPtr &paths)
	:m_stack(paths)
{
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
