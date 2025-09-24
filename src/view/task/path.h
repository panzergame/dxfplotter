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
	model::Application &m_app;

	std::unique_ptr<model::PathGroupSettings> m_groupSettings;

	void selectionChanged(bool empty);
	void toolChanged();
	void configChanged();

	void updateFieldVisibility(const config::Tools::Tool& tool);

	template <typename ValueType, class Field>
	void connectOnFieldChanged(Field *field, std::function<void (ValueType)> &&func)
	{
		disconnect(field, static_cast<void (Field::*)(ValueType)>(&Field::valueChanged), nullptr, nullptr);

		connect(field, static_cast<void (Field::*)(ValueType)>(&Field::valueChanged), [this, func](ValueType value){
			func(value);

			m_app.takeDocumentSnapshot();
		});
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
