#pragma once

#include <QObject>

#include <model/application.h>

namespace Model
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

public:
	explicit DocumentModelObserver(Application &app)
		:m_document(nullptr)
	{
		QObject::connect(&app, &Application::documentChanged, this, &DocumentModelObserver::internalDocumentChanged);
	}

};

}

