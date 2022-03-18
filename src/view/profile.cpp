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
	m_app(app),
	m_outsideToolChangeBlocked(false),
	m_outsideProfileChangeBlocked(false)
{
	setupUi(this);

	updateAllComboBoxesItems();

	connect(&app, &model::Application::configChanged, this, &Profile::configChanged);
	connect(toolComboBox, &QComboBox::currentTextChanged, this, &Profile::currentToolTextChanged);
	connect(profileComboBox, &QComboBox::currentTextChanged, this, &Profile::currentProfileTextChanged);
}

void Profile::documentChanged()
{
	connect(document(), &model::Document::toolConfigChanged, this, &Profile::toolConfigChanged);
	connect(document(), &model::Document::profileConfigChanged, this, &Profile::profileConfigChanged);

	toolComboBox->setCurrentText(QString::fromStdString(document()->toolConfig().name()));
	profileComboBox->setCurrentText(QString::fromStdString(document()->profileConfig().name())); // TODO updateTextFromProfileConfig
}

void Profile::configChanged([[maybe_unused]] const config::Config &config)
{
	updateAllComboBoxesItems();
}

void Profile::toolConfigChanged(const config::Tools::Tool& tool)
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

void Profile::profileConfigChanged(const config::Profiles::Profile& profile)
{
	if (!m_outsideProfileChangeBlocked) {
		profileComboBox->setCurrentText(QString::fromStdString(profile.name()));
	}
}

void Profile::currentProfileTextChanged(const QString& profileName)
{
	m_outsideProfileChangeBlocked = true;

	m_app.selectProfile(profileName);

	m_outsideProfileChangeBlocked = false;
}

}
