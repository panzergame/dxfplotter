#pragma once

#include <model/path.h>

namespace Model
{

class Task : public QObject
{
	Q_OBJECT;

private:
	Path::ListPtr m_stack;

	Path::SetPtr m_selectedPaths;

public:
	enum class MoveDirection
	{
		UP = -1,
		DOWN = 1
	};

	explicit Task(QObject *parent, const Path::ListPtr &paths);

	int count() const;
	Path *pathAt(int index) const;
	int indexFor(Path *path) const;

	void movePath(int index, MoveDirection direction);

	template <class Functor>
	void forEachPath(Functor &&functor) const
	{
		for (Path *path : m_stack) {
			functor(path);
		}
	}

	template <class Functor>
	void forEachSelectedPath(Functor &&functor) const
	{
		for (Path *path : m_selectedPaths) {
			functor(path);
		}
	}

Q_SIGNALS:
	void pathSelectedChanged(Path *path, bool selected);
	void selectionChanged(int size);
};

template <typename T>
inline T operator+(const T &a, const Task::MoveDirection& direction)
{
	return a + static_cast<int>(direction);
}

}

