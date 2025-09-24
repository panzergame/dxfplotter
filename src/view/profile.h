#pragma once

#include <uic/ui_profile.h>

#include <model/documentmodelobserver.h>


namespace view
{

class Profile : public model::DocumentModelObserver<QWidget>, public Ui::Profile
{
private:
	model::Application &m_app;

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
	explicit Profile(model::Application &app);

protected:
	void documentChanged() override;

public Q_SLOTS:
	void configChanged(const config::Config &config);
	void currentToolTextChanged(const QString &toolName);
	void currentProfileTextChanged(const QString &profileName);
};

}
