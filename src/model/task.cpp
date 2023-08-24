#include <task.h>

#include <iterator>
#include <common/copy.h>

namespace model
{

void Task::initPathsFromLayers()
{
	for (const Layer::UPtr &layer : m_layers) {
		layer->forEachChild([this](Path &path){ m_paths.push_back(&path); });
	}

	// Register selection/deselection on all paths.
	forEachPath([this](Path &path) {
		connect(&path, &Path::selectedChanged, this, [this, &path](bool selected){
			emit pathSelectedChanged(path, selected);
			emit selectionChanged(pathSelectionEmpty());
		});
	});
}

bool Task::pathSelectionEmpty() const
{
	return !std::any_of(m_paths.begin(), m_paths.end(), [](const Path *path) {
		return path->selected();
	});
}

Task::Task(Layer::ListUPtr &&layers)
	:m_layers(std::move(layers))
{
	initPathsFromLayers();

	m_stack = m_paths;
}

Task::Task(const Task &other)
	:QObject(),
	m_layers(common::deepcopy<Layer>(other.m_layers)),
	m_stack(other.m_stack.size())
{
	initPathsFromLayers();

	// Remap pointers of path on stack
	std::unordered_map<Path *, Path *> pathRemapping;
	for (Path::ListPtr::const_iterator ito = other.m_paths.begin(), it = m_paths.begin(), end = m_paths.end(); it != end; ++it, ++ito) {
		pathRemapping.insert({*ito, *it});
	}

	std::transform(other.m_stack.begin(), other.m_stack.end(), m_stack.begin(), [&pathRemapping](Path *path){
		return pathRemapping.find(path)->second;
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
	const Path::ListPtr::const_iterator it = std::find(m_stack.cbegin(), m_stack.cend(), &path);

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

void Task::movePathToTip(int index, MoveTip tip)
{
	assert(0 <= index && index < pathCount());

	Path *path = m_stack[index];
	m_stack.erase(m_stack.begin() + index);

	switch (tip) {
		case MoveTip::Bottom:
		{
			m_stack.push_back(path);
			break;
		}
		case MoveTip::Top:
		{
			m_stack.insert(m_stack.begin(), path);
			break;
		}
	}
}

void Task::resetCutterCompensationSelection()
{
	forEachSelectedPath([](model::Path &path){ path.resetOffset(); });
}

void Task::cutterCompensationSelection(float scaledRadius, float minimumPolylineLength, float minimumArcLength)
{
	forEachSelectedPath([scaledRadius, minimumPolylineLength, minimumArcLength](Path &path){
		path.offset(scaledRadius, minimumPolylineLength, minimumArcLength);
	});
}

void Task::pocketSelection(float radius, float minimumPolylineLength, float minimumArcLength)
{
	const Path::ListPtr::iterator it = m_paths.begin();
	const Path::ListPtr::iterator end = m_paths.end();

	const auto isSelectedPred = [](const Path* path){ return path->selected(); };

	const Path::ListPtr::iterator borderIt = std::find_if(it, end, isSelectedPred);
	if (borderIt == end) {
		// No selected path found
		return;
	}

	Path::ListCPtr islands;
	std::copy_if(borderIt + 1, end, std::back_inserter(islands), isSelectedPred);

	Path *border = *borderIt;
	border->pocket(islands, radius, minimumPolylineLength, minimumArcLength);
}

void Task::transformSelection(const QTransform& matrix)
{
	forEachSelectedPath([&matrix](Path &path){ path.transform(matrix); });
}

void Task::hideSelection()
{
	forEachSelectedPath([](Path &path){
		path.setVisible(false);
	});
}

void Task::showHidden()
{
	forEachPath([](Path &path){
		if (!path.visible()) {
			path.setVisible(true);
			path.setSelected(true);
		}
	});
}

geometry::Rect Task::selectionBoundingRect() const
{
	bool isFirstPath = true;
	geometry::Rect boundingRect;

	forEachSelectedPath([&isFirstPath, &boundingRect](Path &path){
		const geometry::Rect pathBoundingRect = path.boundingRect();
		if (isFirstPath) {
			boundingRect = pathBoundingRect;
			isFirstPath = false;
		}
		else {
			boundingRect |= pathBoundingRect;
		}
	});

	return boundingRect;
}

geometry::Rect Task::visibleBoundingRect() const
{
	bool isFirstPath = true;
	geometry::Rect boundingRect;

	forEachPathInStack([&isFirstPath, &boundingRect](const model::Path &path){
		if (path.globallyVisible()) {
			const geometry::Rect pathBoundingRect = path.boundingRect();
			if (isFirstPath) {
				boundingRect = pathBoundingRect;
				isFirstPath = false;
			}
			else {
				boundingRect |= pathBoundingRect;
			}
		}
	});

	return boundingRect;
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
	const Layer::ListUPtr::const_iterator it = std::find_if(m_layers.cbegin(), m_layers.cend(),
			[&layer](const Layer::UPtr &ptr) { return ptr.get() == &layer; });

	assert(it != m_layers.cend());

	return std::distance(m_layers.cbegin(), it);
}

std::pair<int, int> Task::layerAndPathIndexFor(const Path &path) const
{
	for (int layerIndex = 0, size = m_layers.size(); layerIndex < size; ++layerIndex) {
		const Layer &layer = *m_layers[layerIndex];
		const int childIndex = layer.childIndexFor(path);
		if (childIndex != -1) {
			return std::make_pair(layerIndex, childIndex);
		}
	}

	assert(false && "layer not found");

	return std::make_pair(-1, -1);
}

}
