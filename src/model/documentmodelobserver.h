#pragma once

#include <QObject>

#include <model/application.h>

namespace model
{

class Document;

/** @brief A observer on Document changes
 * e.g when a file is opened the document is changed and this observers is notified.
 * Function @ref documentChanged is called at every changes
 */
template <class QtBaseObject>
class DocumentModelObserver : public QtBaseObject
{
private:
	/// Current document.
	Document *m_document;

	/** Function called when the document in the application had changed,
	 * m_document is ensured to be valid before.
	 * Before the first call to this function, m_document is inaccesible.
	 */
	virtual void documentChanged() = 0;

	/// Notify the document was changed after an undo or redo operation
	virtual void documentRestoredFromHistory()
	{
	}

	/// Notify the document was changed after an opening operation
	virtual void newDocumentOpened()
	{
	}

protected:
	Document *document() const
	{
		return m_document;
	}

	Task &task()
	{
		assert(m_document);
		return m_document->task();
	}

private Q_SLOTS:
	void internalDocumentChanged(Document *newDocument)
	{
		m_document = newDocument;
		assert(m_document);
		documentChanged();
	}

	void internalDocumentRestoredFromHistory(Document *newDocument)
	{
		internalDocumentChanged(newDocument);
		documentRestoredFromHistory();
	}

	void internalNewDocumentOpened(Document *newDocument)
	{
		internalDocumentChanged(newDocument);
		newDocumentOpened();
	}

public:
	explicit DocumentModelObserver(Application &app)
		:m_document(nullptr)
	{
		QObject::connect(&app, &Application::documentRestoredFromHistory, this, &DocumentModelObserver::internalDocumentRestoredFromHistory);
		QObject::connect(&app, &Application::newDocumentOpened, this, &DocumentModelObserver::internalNewDocumentOpened);
	}

};

}

