module;

#include <uic/ui_path.h>
#include <QWidget>

export module view.task.path;
import model.application;

import model.documentmodelobserver;
import model.pathgroupsettings;
import model.task;
import config.config;

export namespace view::task
{

class Path : public model::DocumentModelObserver<QWidget>, private Ui::Path
{
private:
	model::Application& m_app;

	std::unique_ptr<model::PathGroupSettings> m_groupSettings;

	void selectionChanged(bool empty)
	{
		if (empty) {
			stackedWidget->setCurrentWidget(pageNoSelection);
		} else {
			updateFieldValue(planeFeedRate, m_groupSettings->planeFeedRate());
			updateFieldValue(depthFeedRate, m_groupSettings->depthFeedRate());
			updateFieldValue(intensity, m_groupSettings->intensity());
			updateFieldValue(Ui::Path::depth, m_groupSettings->depth());

			stackedWidget->setCurrentWidget(pagePathSelected);
		}
	}

	void toolChanged()
	{
		if (document()) {
			updateFieldVisibility(document()->toolConfig());
		}
	}

	void configChanged()
	{
		if (document()) {
			updateFieldVisibility(document()->toolConfig());
		}
	}

	void updateFieldVisibility(const config::Tools::Tool& tool)
	{
		const bool toolHasDepth = !tool.general().laser();
		Ui::Path::depth->setVisible(toolHasDepth);
		depthLabel->setVisible(toolHasDepth);
	}

	template<typename ValueType, class Field>
	void connectOnFieldChanged(Field* field, std::function<void(ValueType)>&& func)
	{
		disconnect(field, static_cast<void (Field::*)(ValueType)>(&Field::valueChanged), nullptr, nullptr);

		connect(field, static_cast<void (Field::*)(ValueType)>(&Field::valueChanged), [this, func](ValueType value) {
			func(value);

			m_app.takeDocumentSnapshot();
		});
	}

	template<class Field, typename T>
	void updateFieldValue(Field* field, std::optional<T>&& valueOpt)
	{
		if (valueOpt) {
			field->setValue(*valueOpt);
		} else {
			field->clear();
		}
	}

	void setupModel()
	{
		m_groupSettings.reset(new model::PathGroupSettings(task()));

		stackedWidget->setCurrentWidget(pageNoSelection);

		connect(&task(), &model::Task::selectionChanged, this, &Path::selectionChanged);

		connectOnFieldChanged<double>(planeFeedRate, [this](double value) {
			m_groupSettings->setPlaneFeedRate(value);
		});
		connectOnFieldChanged<double>(depthFeedRate, [this](double value) {
			m_groupSettings->setDepthFeedRate(value);
		});
		connectOnFieldChanged<double>(intensity, [this](double value) {
			m_groupSettings->setIntensity(value);
		});
		connectOnFieldChanged<double>(Ui::Path::depth, [this](double value) {
			m_groupSettings->setDepth(value);
		});
	}

protected:
	void documentChanged() override
	{
		setupModel();
		updateFieldVisibility(document()->toolConfig());
	}

public:
	explicit Path(model::Application& app)
		: DocumentModelObserver(app)
		, m_app(app)
	{
		setupUi(this);
		connect(&m_app, &model::Application::toolChanged, this, &Path::toolChanged);
		connect(&m_app, &model::Application::configChanged, this, &Path::configChanged);
	}
};

}
