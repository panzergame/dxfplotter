#pragma once

#include <uic/ui_path.h>

#include <model/pathgroupsettings.h>
#include <model/task.h>
#include <model/documentmodelobserver.h>

#include <QWidget>

namespace view::task
{

class Path : public model::DocumentModelObserver<QWidget>, private Ui::Path
{
private:
	std::unique_ptr<model::PathGroupSettings> m_groupSettings;

	void selectionChanged(bool empty);

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
	void documentChanged() override;

public:
	explicit Path(model::Application &app);

};

}
