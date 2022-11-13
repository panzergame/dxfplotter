#include <path.h>

namespace view::task
{

void Path::setupModel()
{
	m_groupSettings.reset(new model::PathGroupSettings(task()));

	hide();

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
	:DocumentModelObserver(app)
{
	setupUi(this);
}

void Path::selectionChanged(bool empty)
{
	if (empty) {
		hide();
	}
	else {
		show();

		updateFieldValue(planeFeedRate, m_groupSettings->planeFeedRate());
		updateFieldValue(depthFeedRate, m_groupSettings->depthFeedRate());
		updateFieldValue(intensity, m_groupSettings->intensity());
		updateFieldValue(Ui::Path::depth, m_groupSettings->depth());
	}
}

}
