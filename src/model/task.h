#pragma once

#include <model/path.h>

namespace Model
{

class Task : public QObject
{
	Q_OBJECT;

private:
	Path::ListPtr m_stack;

	Path::ListPtr m_selectedPaths;

public:
	explicit Task(QObject *parent, const Path::ListPtr &paths);

	int count() const;
	Path *pathAt(int index) const;
	int indexFor(Path *path) const;

	const Path::ListPtr& selectedPaths() const; // TODO avoid

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
	void pathSelected(Path *path);
	void pathDeselected(Path *path);
	void selectionChanged(int size);
};

}

