#include <path.h>

namespace view::task
{

void Path::setupModel()
{
	m_groupSettings.reset(new model::PathGroupSettings(task()));

	stackedWidget->setCurrentWidget(pageNoSelection);

	connect(&task(), &model::Task::selectionChanged, this, &Path::selectionChanged);

	connectOnFieldChanged<double>(planeFeedRate, [this](double value) { m_groupSettings->setPlaneFeedRate(value); });
	connectOnFieldChanged<double>(depthFeedRate, [this](double value) { m_groupSettings->setDepthFeedRate(value); });
	connectOnFieldChanged<double>(intensity, [this](double value) { m_groupSettings->setIntensity(value); });
	connectOnFieldChanged<double>(Ui::Path::depth, [this](double value) { m_groupSettings->setDepth(value); });
}

void Path::documentChanged()
{
	setupModel();
}

Path::Path(model::Application &app)
	:DocumentModelObserver(app),
	m_app(app)
{
	setupUi(this);
}

void Path::selectionChanged(bool empty)
{
	if (empty) {
		stackedWidget->setCurrentWidget(pageNoSelection);
	}
	else {
		updateFieldValue(planeFeedRate, m_groupSettings->planeFeedRate());
		updateFieldValue(depthFeedRate, m_groupSettings->depthFeedRate());
		updateFieldValue(intensity, m_groupSettings->intensity());
		updateFieldValue(Ui::Path::depth, m_groupSettings->depth());

		stackedWidget->setCurrentWidget(pagePathSelected);
	}
}

}
