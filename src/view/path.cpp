#include <path.h>

namespace View
{

void Path::setupModel()
{
	m_groupSettings.reset(new Model::PathGroupSettings(m_task));

	hide();

	connect(m_groupSettings.get(), &Model::PathGroupSettings::selectionChanged, this, &Path::selectionChanged);

	connect(feedRate, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [this](double value) {
		m_groupSettings->setFeedRate(value);
	});
	// TODO intensity
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

		updateValue(feedRate, m_groupSettings->feedRate());
		updateValue(intensity, m_groupSettings->intensity());
	}
	else {
		hide();
	}
}

}
