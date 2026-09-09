module;

#include <uic/ui_profile.h>

export module view.profile;

import config.config;
import model.documentmodelobserver;
import model.application;

export namespace view
{

class Profile : public model::DocumentModelObserver<QWidget>, public Ui::Profile
{
private:
	model::Application& m_app;

	template<typename ConfigList>
	void updateComboBoxItems(const ConfigList& list, QComboBox* comboBox)
	{
		// Keep current item selected.
		const QString& currentItemName = comboBox->currentText();

		comboBox->clear();

		list.visitChildren([comboBox](const auto& item) {
			const QString name = QString::fromStdString(item.name());
			comboBox->addItem(name, name);
		});

		// Try to restore selected tool name
		comboBox->setCurrentText(currentItemName);
	}

	void updateAllComboBoxesItems()
	{
		updateComboBoxItems(m_app.config().root().tools(), toolComboBox);
		updateComboBoxItems(m_app.config().root().profiles(), profileComboBox);
	}

public:
	explicit Profile(model::Application& app)
		: DocumentModelObserver(app)
		, m_app(app)
	{
		setupUi(this);

		updateAllComboBoxesItems();

		connect(&app, &model::Application::configChanged, this, &Profile::configChanged);
		connect(toolComboBox, &QComboBox::currentTextChanged, this, &Profile::currentToolTextChanged);
		connect(profileComboBox, &QComboBox::currentTextChanged, this, &Profile::currentProfileTextChanged);
	}

protected:
	void documentChanged() override
	{
		toolComboBox->setCurrentText(QString::fromStdString(document()->toolConfig().name()));
		profileComboBox->setCurrentText(
			QString::fromStdString(document()->profileConfig().name())); // TODO updateTextFromProfileConfig
	}

public Q_SLOTS:
	void configChanged([[maybe_unused]] const config::Config& config) { updateAllComboBoxesItems(); }

	void currentToolTextChanged(const QString& toolName) { m_app.selectTool(toolName); }

	void currentProfileTextChanged(const QString& profileName) { m_app.selectProfile(profileName); }
};

}
