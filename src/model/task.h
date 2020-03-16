#pragma once

#include <model/path.h>

namespace Model
{

class Task : public QObject
{
	Q_OBJECT;

private:
	Path::ListPtr m_stack;

public:
	explicit Task(QObject *parent, const Path::ListPtr &paths);

	int count() const;
	Path *pathAt(int index) const;

	template <class Functor>
	void forEachPath(Functor &&functor) const
	{
		for (Path *path : m_stack) {
			functor(path);
		}
	}

Q_SIGNALS:
	void pathSelected(Path *path);
	void pathDeselected(Path *path);
};

}

