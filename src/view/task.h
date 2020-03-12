#pragma once

#include <ui_task.h>

#include <control/application.h>

#include <QWidget>

namespace View
{

class Task : public QWidget, private Ui::Task
{
private:
	Control::Application &m_app;

	void setupModel();

public:
	explicit Task(Control::Application &app);

protected Q_SLOTS:
	void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
};

}
