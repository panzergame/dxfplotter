#include <path.h>

namespace View
{

Path::Path(const Model::Task &task)
	:m_groupSettings(task)
{
	setupUi(this);
	hide();

	connect(&m_groupSettings, &Model::PathGroupSettings::selectionChanged, this, &Path::selectionChanged);
	connect(feedRate, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this](double value) {
		m_groupSettings.setFeedRate(value);
	});
}

void Path::selectionChanged(int size)
{
	if (size > 0) {
		show();

		updateValue(feedRate, m_groupSettings.feedRate());
		updateValue(intensity, m_groupSettings.intensity());
	}
	else {
		hide();
	}
}

}
