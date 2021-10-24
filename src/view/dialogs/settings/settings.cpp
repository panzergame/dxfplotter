#include <settings.h>
#include <treemodel.h>
#include <group.h>
#include <list.h>

#include <QVBoxLayout>
#include <QInputDialog>
#include <QDebug> // TODO

namespace View::Settings
{

/// Node visitor selecting list UI or group UI
class Settings::NodeVisitor
{
private:
	QWidget *m_newWidget;

public:
	template <class ... Child>
	void operator()(Config::Group<Child ...> &node)
	{
		m_newWidget = new Group(node);
	}

	template <class Child>
	void operator()(Config::List<Child> &node)
	{
		m_newWidget = new List(node);
	}

	QWidget *newWidget() const
	{
		return m_newWidget;
	}
};

void Settings::setupUi()
{
	Ui::Settings::setupUi(this);

	treeView->setModel(m_model.get());
	treeView->expandAll();
	treeView->resizeColumnToContents(0);
}

Settings::Settings(Model::Application &app)
	:m_app(app),
	m_newConfig(app.config()),
	m_model(new TreeModel(m_newConfig.root(), this))
{
	setupUi();

	connect(treeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &Settings::currentChanged);
}

Settings::~Settings() = default;

Config::Config &&Settings::newConfig()
{
	return std::move(m_newConfig);
}

void Settings::currentChanged(const QModelIndex &current, const QModelIndex &)
{
	// Create new widget to be displayed at center
	NodeVisitor visitor;
	m_model->visit(current, visitor);

	// Replace old center widget
	QWidget *newWidget = visitor.newWidget();
	QLayoutItem *item = gridLayout->replaceWidget(center, newWidget);
	center = newWidget;

	// Delete old center widget
	delete item->widget();

	const bool isList = m_model->isList(current);
	addButton->disconnect();
	addButton->setEnabled(isList);

	if (isList) {
		connect(addButton, &QPushButton::pressed, this, [current, this](){ addItem(current); });
	}

	const bool isItem = m_model->isItem(current);
	removeButton->disconnect();
	removeButton->setEnabled(isItem);

	if (isItem) {
		connect(removeButton, &QPushButton::pressed, this, [current, this](){ removeItem(current); });
	}
}

void Settings::addItem(const QModelIndex &index)
{
	bool ok;
	const QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
		tr("Tool name:"), QLineEdit::Normal, "", &ok);

	if (ok && !text.isEmpty()) {
		m_model->addItem(index, text);
	}
}

void Settings::removeItem(const QModelIndex &index)
{
	m_model->removeItem(index);
}

}
