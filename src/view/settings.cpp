#include <settings.h>

#include <QTabWidget>
#include <QFormLayout>

#include <QDebug> // TODO

namespace View
{

/** @brief Visitor populating an entry for each configuration variable.
 */
/*class VariableVisitor
{
private:
	QWidget *m_box;
	QFormLayout *m_layout;
	ISettingEntry::ListPtr &m_entries;

public:
	explicit VariableVisitor(QWidget *box, QFormLayout *layout, ISettingEntry::ListPtr &entries)
		:m_box(box),
		m_layout(layout),
		m_entries(entries)
	{
	}

	void operator()(Config::Variable &variable)
	{
		SettingEntry<VariableType> *entry = new SettingEntry<VariableType>(variable, m_box);
		m_entries.push_back(entry);

		m_layout->addRow(QString::fromStdString(variable.description()), entry);
	}
};*/

/** @brief Visitor populating a tab for each configuration section.
 */
/*class SectionVisitor
{
private:
	QTabWidget *m_tab;
	ISettingEntry::ListPtr &m_entries;

public:
	explicit SectionVisitor(QTabWidget *tab, ISettingEntry::ListPtr &entries)
		:m_tab(tab),
		m_entries(entries)
	{
	}

	template <class Section>
	void operator()(Section &section)
	{
		// Create tab box and layout
		QWidget *box = new QWidget(m_tab);
		QFormLayout *layout = new QFormLayout(box);
		box->setLayout(layout);

		// Populate form layout with variables
		VariableVisitor visitor(box, layout, m_entries);
		section.visitVariables(visitor);

		// Add tab to the switcher widget
		m_tab->addTab(box, QString::fromStdString(section.description()));
	}
};*/

void Settings::setupUi()
{
	Ui::Settings::setupUi(this);

	QTabWidget *tab = new QTabWidget(this);

// 	SectionVisitor visitor(tab, m_entries);
	// Populate tab and entry.
// 	m_config.visitSections(visitor);

	verticalLayout->addWidget(tab);
}

void Settings::save()
{
	for (ISettingEntry *entry : m_entries) {
		entry->save();
	}

	// Ensure configuration is saved to file
	m_config.save();
}

Settings::Settings(Config::Config &config)
	:m_config(config)
{
	setupUi();

	connect(this, &QDialog::accepted, this, &Settings::save);
}

}
