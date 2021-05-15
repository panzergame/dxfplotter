#pragma once

#include <uic/ui_profile.h>

#include <model/documentmodelobserver.h>

namespace View
{

class Profile : public Model::DocumentModelObserver<QWidget>, public Ui::Profile
{
private:
	Model::Application &m_app;

	bool m_outsideToolChangeBlocked;
	bool m_outsideProfileChangeBlocked;

	template <typename ConfigList>
	void updateComboBoxItems(const ConfigList &list, QComboBox *comboBox)
	{
		// Keep current item selected.
		const QString &currentItemName = comboBox->currentText();

		comboBox->clear();

		list.visitChildren([comboBox](const auto &item){
			const QString name = QString::fromStdString(item.name());
			comboBox->addItem(name, name);
		});

		// Try to restore selected tool name
		comboBox->setCurrentText(currentItemName);
	}

	void updateAllComboBoxesItems();

public:
	explicit Profile(Model::Application &app);

protected:
	void documentChanged() override;

public Q_SLOTS:
	void configChanged(const Config::Config &config);
	void toolConfigChanged(const Config::Tools::Tool &tool);
	void currentToolTextChanged(const QString &toolName);
	void profileConfigChanged(const Config::Profiles::Profile &profile);
	void currentProfileTextChanged(const QString &profileName);
};

}
