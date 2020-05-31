#include <path.h>

namespace View
{

void Path::setupModel()
{
	m_groupSettings.reset(new Model::PathGroupSettings(m_task));

	hide();

	connect(m_task, &Model::Task::selectionChanged, this, &Path::selectionChanged);

	connectOnFieldChanged<double>(feedRate, [this](double value) { m_groupSettings->setFeedRate(value); });
	connectOnFieldChanged<double>(intensity, [this](double value) { m_groupSettings->setIntensity(value); });
	connectOnFieldChanged<int>(passes, [this](int value) { m_groupSettings->setPasses(value); });
}

void Path::taskChanged()
{
	setupModel();
}

Path::Path(Model::Application &app)
	:TaskModelObserver(app)
{
	setupUi(this);
}

void Path::selectionChanged(int size)
{
	if (size > 0) {
		show();

		updateFieldValue(feedRate, m_groupSettings->feedRate());
		updateFieldValue(intensity, m_groupSettings->intensity());
		updateFieldValue(passes, m_groupSettings->passes());
	}
	else {
		hide();
	}
}

}
