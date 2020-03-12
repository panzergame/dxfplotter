#include <path.h>

namespace View
{

Path::Path(Control::Application &app)
	:m_app(app)
{
	setupUi(this);

// 	connect(&m_app, &Control::Application::pathSelected, this, &Path::pathSelected);
}

void Path::pathSelected(Model::Path *path)
{
	name->setText(QString::fromStdString(path->name()));
	const Model::Path::Settings &settings = path->settings();
	feedRate->setValue(settings.feedRate());
	intensity->setValue(settings.intensity());
}

}
