#include <profile.h>

#include <config/config.h>

namespace View
{

Profile::Profile(Model::Application& app)
{
	setupUi(this);

	updateToolSelector(app.config());

	connect(&app, &Model::Application::configChanged, this, &Profile::configChanged);
	connect(toolComboBox, &QComboBox::currentTextChanged, &app, &Model::Application::selectTool);
}


void Profile::updateToolSelector(const Config::Config &config)
{
	// Keep current tool selected.
	const QString &currentToolName = toolComboBox->currentText();

	toolComboBox->clear();

	const Config::Tools &tools = config.root().tools();
	tools.visitChildren([this](const auto &tool){
		const QString name = QString::fromStdString(tool.name());
		toolComboBox->addItem(name, name);
	});

	// Try to restore selected tool name
	toolComboBox->setCurrentText(currentToolName);
}

void Profile::configChanged(const Config::Config &config)
{
	updateToolSelector(config);
}


}
