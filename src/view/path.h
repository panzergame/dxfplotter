#pragma once

#include <uic/ui_path.h>

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

	template <typename ValueType, class Field>
	void connectOnFieldChanged(Field *field, std::function<void (ValueType)> &&func)
	{
		connect(field, static_cast<void (Field::*)(ValueType)>(&Field::valueChanged), this, func);
	}

	template <class Field, typename T>
	void updateFieldValue(Field *field, std::optional<T> &&valueOpt)
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
