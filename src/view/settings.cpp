#include <settings.h>
#include <settingtreemodel.h>

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

/** @brief Visitor populating form for each variable of configuration section.
 */
/*class Visitor
{
private:
	QFormLayout *m_layout;
	ISettingEntry::ListPtr &m_entries;

public:
	explicit SectionVisitor(ISettingEntry::ListPtr &entries)
		:m_layout(new QFormLayout()),
		m_entries(entries)
	{
	}

	void operator()(Config::Node &variable)
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

	treeView->setModel(m_model.get());
	treeView->expandAll();
}

void Settings::save()
{
	for (ISettingEntry *entry : m_entries) {
		entry->save();
	}

	// Ensure configuration is saved to file
	m_config.save();
}

ISettingEntry *Settings::createEntry(Config::Variable &variable, QWidget *parent) const
{
	switch (variable.type()) {
		case Config::Variable::Type::INT:
		{
			return new SettingEntry<int>(variable, parent);
		}
		case Config::Variable::Type::FLOAT:
		{
			return new SettingEntry<float>(variable, parent);
		}
		case Config::Variable::Type::STRING:
		{
			return new SettingEntry<std::string>(variable, parent);
		}
		default:
		{
			return nullptr;
		}
	}
}

Settings::Settings(Config::Config &config)
	:m_config(config),
	m_model(new SettingTreeModel(m_config.root(), this))
{
	setupUi();

	connect(treeView, &QTreeView::clicked, this, &Settings::clicked);
	connect(this, &QDialog::accepted, this, &Settings::save);
}

void Settings::clicked(const QModelIndex &index)
{
	Config::Section *section = m_model->section(index);

	if (section) {
		// Save previous values
		save();

		// Cmear all entries, they are destructed when removing form rows
		m_entries.clear();

		// Delete all items
		for (int i = 0, size = formLayout->rowCount(); i < size; ++i) {
			formLayout->removeRow(0);
		}

		// Insert new entries
		section->visitVariables([this](Config::Variable &variable){
			ISettingEntry *entry = createEntry(variable, center);
			QWidget *item = dynamic_cast<QWidget *>(entry);

			formLayout->addRow(QString::fromStdString(variable.description()), item);
			m_entries.push_back(entry);
		});
	}
}

}
