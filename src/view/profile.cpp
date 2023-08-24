#include <profile.h>

#include <config/config.h>


namespace view
{

void Profile::updateAllComboBoxesItems()
{
	updateComboBoxItems(m_app.config().root().tools(), toolComboBox);
	updateComboBoxItems(m_app.config().root().profiles(), profileComboBox);
}

Profile::Profile(model::Application& app)
	:DocumentModelObserver(app),
	m_app(app)
{
	setupUi(this);

	updateAllComboBoxesItems();

	connect(&app, &model::Application::configChanged, this, &Profile::configChanged);
	connect(toolComboBox, &QComboBox::currentTextChanged, this, &Profile::currentToolTextChanged);
	connect(profileComboBox, &QComboBox::currentTextChanged, this, &Profile::currentProfileTextChanged);
}

void Profile::documentChanged()
{
	toolComboBox->setCurrentText(QString::fromStdString(document()->toolConfig().name()));
	profileComboBox->setCurrentText(QString::fromStdString(document()->profileConfig().name())); // TODO updateTextFromProfileConfig
}

void Profile::configChanged([[maybe_unused]] const config::Config &config)
{
	updateAllComboBoxesItems();
}

void Profile::currentToolTextChanged(const QString& toolName)
{
	m_app.selectTool(toolName);
}

void Profile::currentProfileTextChanged(const QString& profileName)
{
	m_app.selectProfile(profileName);
}

}
