#pragma once

#include <ui_path.h>

#include <model/pathgroupsettings.h>
#include <model/task.h>

#include <QWidget>

namespace View
{

class Path : public QWidget, private Ui::Path
{
private:
	Model::PathGroupSettings m_groupSettings;

	void selectionChanged(int size);

	template <typename T>
	void updateValue(QDoubleSpinBox *field, std::optional<T> &&valueOpt)
	{
		if (valueOpt) {
			field->setValue(*valueOpt);
		}
		else {
			field->clear();
		}
	}

public:
	explicit Path(const Model::Task &task);

};

}
