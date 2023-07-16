#include <settings.h>
#include <treemodel.h>
#include <group.h>
#include <list.h>

#include <QVBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug> // TODO

namespace view::settings
{

/// Node visitor selecting list UI or group UI
class Settings::NodeVisitor
{
private:
	QWidget *m_newWidget;

public:
	template <class ... Child>
	void operator()(config::Group<Child ...> &node)
	{
		m_newWidget = new Group(node);
	}

	template <class Child>
	void operator()(config::List<Child> &node)
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

Settings::Settings(config::Config &newConfig)
	:m_newConfig(newConfig),
	m_model(new TreeModel(m_newConfig.root(), this))
{
	setupUi();

	connect(treeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &Settings::currentChanged);
}

Settings::~Settings() = default;

void Settings::currentChanged(const QModelIndex &current, const QModelIndex &)
{
	// Create new widget to be displayed at center
	NodeVisitor visitor;
	m_model->visit(current, visitor);

	// Replace old properties widget
	QWidget *newWidget = visitor.newWidget();
	QWidget *oldWidget = center->findChild<QWidget *>(QString(), Qt::FindDirectChildrenOnly);
	center->layout()->replaceWidget(oldWidget, newWidget);
	delete oldWidget;

	const bool isList = m_model->isList(current);
	addButton->disconnect();
	addButton->setEnabled(isList);

	if (isList) {
		connect(addButton, &QPushButton::pressed, this, [current, this](){ addItem(current); });
	}

	const bool isItem = m_model->isItem(current);
	removeButton->disconnect();
	removeButton->setEnabled(isItem);
	copyButton->disconnect();
	copyButton->setEnabled(isItem);

	if (isItem) {
		connect(removeButton, &QPushButton::pressed, this, [current, this](){ removeItem(current); });
		connect(copyButton, &QPushButton::pressed, this, [current, this](){ copyItem(current); });
	}
}

void Settings::addItem(const QModelIndex &index)
{
	bool ok;
	const QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
		tr("New name:"), QLineEdit::Normal, "", &ok);

	if (ok) {
		if (text.isEmpty()) {
			QMessageBox::critical(this, "Error", "Invalid name");
		}
		else {
			m_model->addItem(index, text);
		}
	}
}

void Settings::removeItem(const QModelIndex &index)
{
	m_model->removeItem(index);
}

void Settings::copyItem(const QModelIndex &index)
{
	const QString sourceName = m_model->data(index).toString();

	bool ok;
	const QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
		tr("New name:"), QLineEdit::Normal, sourceName, &ok);

	if (ok) {
		if (text.isEmpty() || text == sourceName) {
			QMessageBox::critical(this, "Error", "Invalid name");
		}
		else {
			m_model->copyItem(index, text);
		}
	}
}

}
