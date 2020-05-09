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
		connect(path, &Path::selected, this, [this, path](){
			m_selectedPaths.push_back(path);
			emit pathSelected(path);
			emit selectionChanged(m_selectedPaths.size());
		});

		connect(path, &Path::deselected, this, [this, path](){
			Path::ListPtr::const_iterator it = std::find(m_selectedPaths.cbegin(), m_selectedPaths.cend(), path);
			assert(it != m_selectedPaths.cend());
			m_selectedPaths.erase(it);

			emit pathDeselected(path);
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

const Path::ListPtr &Task::selectedPaths() const
{
	return m_selectedPaths;
}

}
