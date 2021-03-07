#include <profile.h>

#include <config/config.h>

namespace View
{

Profile::Profile(Model::Application& app)
	:m_app(app)
{
	setupUi(this);

	updateTools(m_app.config().root().tools());

	connect(&app, &Model::Application::configChanged, this, &Profile::configChanged);
	connect(toolComboBox, &QComboBox::currentTextChanged, this, &Profile::currentToolTextChanged);
	connect(&app, &Model::Application::selectedToolConfigChanged, this, &Profile::selectedToolConfigChanged);
}


void Profile::updateTools(const Config::Tools &tools)
{
	// Keep current tool selected.
	const QString &currentToolName = toolComboBox->currentText();

	toolComboBox->clear();

	tools.visitChildren([this](const auto &tool){
		const QString name = QString::fromStdString(tool.name());
		toolComboBox->addItem(name, name);
	});

	// Try to restore selected tool name
	toolComboBox->setCurrentText(currentToolName);
}

void Profile::configChanged(const Config::Config &config)
{
	updateTools(config.root().tools());
}

void Profile::selectedToolConfigChanged(const Config::Tools::Tool& tool)
{
	if (!m_outsideToolChangeBlocked) {
		toolComboBox->setCurrentText(QString::fromStdString(tool.name()));
	}
}

void Profile::currentToolTextChanged(const QString& toolName)
{
	m_outsideToolChangeBlocked = true;

	m_app.selectTool(toolName);

	m_outsideToolChangeBlocked = false;
}

}
