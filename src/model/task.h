#pragma once

#include <model/path.h>
#include <model/layer.h>

namespace Model
{

class Task : public QObject, public Common::Aggregable<Task>
{
	Q_OBJECT;

private:
	Path::ListUPtr m_paths;
	Layer::ListUPtr m_layers;

	Path::ListPtr m_stack;
	Path::SetPtr m_selectedPaths;

public:
	enum class MoveDirection
	{
		UP = -1,
		DOWN = 1
	};

	explicit Task(Path::ListUPtr &&paths, Layer::ListUPtr &&layers);

	int pathCount() const;
	const Path &pathAt(int index) const;
	Path &pathAt(int index);
	int pathIndexFor(const Path &path) const;

	void movePath(int index, MoveDirection direction);

	template <class Functor>
	void forEachPathInStack(Functor &&functor) const
	{
		for (const Path *path : m_stack) {
			functor(*path);
		}
	}
	
	template <class Functor>
	void forEachPath(Functor &&functor)
	{
		for (Path::UPtr &path : m_paths) {
			functor(*path);
		}
	}

	template <class Functor>
	void forEachSelectedPath(Functor &&functor) const
	{
		Path::SetPtr selectedPaths(m_selectedPaths);
		for (Path *path : selectedPaths) {
			functor(*path);
		}
	}

	int layerCount() const;
	const Layer &layerAt(int index) const;
	Layer &layerAt(int index);
	int layerIndexFor(const Layer &layer) const;
	std::pair<int, int> layerAndPathIndexFor(const Path &path) const;

Q_SIGNALS:
	void pathSelectedChanged(Path &path, bool selected);
	void selectionChanged(int size);
};

template <typename T>
inline T operator+(const T &a, const Task::MoveDirection& direction)
{
	return a + static_cast<int>(direction);
}

}

