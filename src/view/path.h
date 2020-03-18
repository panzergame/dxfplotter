#pragma once

#include <ui_path.h>

#include <model/pathgroupsettings.h>
#include <model/task.h>
#include <model/taskmodelobserver.h>

#include <QWidget>

namespace View
{

class Path : public Model::TaskModelObserver<QWidget>, private Ui::Path
{
private:
	std::unique_ptr<Model::PathGroupSettings> m_groupSettings;

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

	void setupModel();

protected:
	void taskChanged() override;

public:
	explicit Path(Model::Application &app);

};

}
