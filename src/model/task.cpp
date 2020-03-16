#include <task.h>

namespace Model
{

Task::Task(QObject *parent, const Path::ListPtr &paths)
	:QObject(parent),
	m_stack(paths)
{
	// Register selection/deselection on all paths.
	forEachPath([this](Path *path) {
		connect(path, &Path::selected, this, [this, path](){ emit pathSelected(path); });
		connect(path, &Path::deselected, this, [this, path](){ emit pathDeselected(path); });
	});
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
