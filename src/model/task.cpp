#include <task.h>

#include <iterator>

namespace Model
{

Task::Task(QObject *parent, const Path::ListPtr &paths)
	:QObject(parent),
	m_stack(paths)
{
	// Register selection/deselection on all paths.
	forEachPath([this](Path *path) {
		connect(path, &Path::selectedChanged, this, [this, path](bool selected){
			if (selected) {
				m_selectedPaths.insert(path);
			}
			else {
				m_selectedPaths.erase(path);
			}

			emit pathSelectedChanged(path, selected);
			emit selectionChanged(m_selectedPaths.size());
		});
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

int Task::indexFor(Path *path) const
{
	Path::ListPtr::const_iterator it = std::find(m_stack.cbegin(), m_stack.cend(), path);

	assert(it != m_stack.cend());

	return std::distance(m_stack.cbegin(), it);
}

void Task::movePath(int index, MoveDirection direction)
{
	assert(0 <= index && index < count());

	const int newIndex = index + direction;

	if (0 <= newIndex && newIndex < count()) {
		std::swap(m_stack[index], m_stack[newIndex]);
	}
}

}
