#include <task.h>

#include <iterator>

namespace Model
{

Task::Task(Path::ListUPtr &&paths, Layer::ListUPtr &&layers)
	:m_paths(std::move(paths)),
	m_layers(std::move(layers)),
	m_stack(m_paths.size())
{
	std::transform(m_paths.begin(), m_paths.end(), m_stack.begin(),
		[](const Path::UPtr& ptr){ return ptr.get(); });

	// Register selection/deselection on all paths.
	forEachPath([this](Path &path) {
		connect(&path, &Path::selectedChanged, this, [this, &path](bool selected){
			if (selected) {
				m_selectedPaths.insert(&path);
			}
			else {
				m_selectedPaths.erase(&path);
			}

			emit pathSelectedChanged(path, selected);
			emit selectionChanged(m_selectedPaths.size());
		});
	});
}

int Task::pathCount() const
{
	return m_paths.size();
}

const Path &Task::pathAt(int index) const
{
	assert(0 <= index && index < pathCount());
	return *m_stack[index];
}

Path &Task::pathAt(int index)
{
	assert(0 <= index && index < pathCount());
	return *m_stack[index];
}

int Task::pathIndexFor(const Path &path) const
{
	Path::ListPtr::const_iterator it = std::find(m_stack.cbegin(), m_stack.cend(), &path);

	assert(it != m_stack.cend());

	return std::distance(m_stack.cbegin(), it);
}

void Task::movePath(int index, MoveDirection direction)
{
	assert(0 <= index && index < pathCount());

	const int newIndex = index + direction;

	if (0 <= newIndex && newIndex < pathCount()) {
		std::swap(m_stack[index], m_stack[newIndex]);
	}
}

int Task::layerCount() const
{
	return m_layers.size();
}

const Layer &Task::layerAt(int index) const
{
	assert(0 <= index && index < layerCount());
	return *m_layers[index];
}

Layer &Task::layerAt(int index)
{
	assert(0 <= index && index < layerCount());
	return *m_layers[index];
}

int Task::layerIndexFor(const Layer &layer) const
{
	Layer::ListUPtr::const_iterator it = std::find_if(m_layers.cbegin(), m_layers.cend(),
			[&layer](const Layer::UPtr &ptr) { return ptr.get() == &layer; });

	assert(it != m_layers.cend());

	return std::distance(m_layers.cbegin(), it);
}

}
