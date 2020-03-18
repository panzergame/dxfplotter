#pragma once

#include <QObject>

#include <model/application.h>

namespace Model
{

class Task;

/** @brief A observer on Task changes
 * e.g when a file is opened the task is changed and this observers is notified.
 * Function @ref taskChanged is called at every changes
 */
template <class QtBaseObject>
class TaskModelObserver : public QtBaseObject
{
protected:
	/// Current task.
	Task *m_task;

	/** Function called when the task in the application had changed,
	 * m_task is ensured to be valid before.
	 * Before the first call to this function, m_task is inaccesible.
	 */
	virtual void taskChanged() = 0;

private Q_SLOTS:
	void internalTaskChanged(Task *newTask)
	{
		m_task = newTask;
		assert(m_task);
		taskChanged();
	}

public:
	explicit TaskModelObserver(Application &app)
		:m_task(nullptr)
	{
		QObject::connect(&app, &Application::taskChanged, this, &TaskModelObserver::internalTaskChanged);
	}

};

}

