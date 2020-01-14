#include <task.h>

#include <tasklistmodel.h>

namespace View
{

Task::Task(Control::Application &app)
	:m_app(app)
{
	setupUi(this);

	setupModel();
}

void Task::setupModel()
{
	treeView->setModel(new TaskListModel(m_app.task(), this));
}

}
