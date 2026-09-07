module;

#include <QDebug>
#include <QObject>

export module model.documenthistory;

import model.document;
import common.aggregable;
export namespace model
{

class DocumentHistory : public common::Aggregable<DocumentHistory>
{
private:
	Document::List m_documentHistory;
	Document::List::iterator m_currentDocumentIt;

	bool isCurrentDocumentLastOfHistory() const;
	bool isCurrentDocumentFirstOfHistory() const;

public:
	explicit DocumentHistory(const Document& initialDocument);

	void takeSnapshot(const Document& currentDocument);
	const Document& undo();
	const Document& redo();
};

}

namespace model
{

bool DocumentHistory::isCurrentDocumentLastOfHistory() const
{
	return m_currentDocumentIt == (m_documentHistory.end() - 1);
}

bool DocumentHistory::isCurrentDocumentFirstOfHistory() const
{
	return m_currentDocumentIt == m_documentHistory.begin();
}

DocumentHistory::DocumentHistory(const Document& initialDocument)
	: m_currentDocumentIt(m_documentHistory.insert(m_documentHistory.end(), initialDocument))
{
}

void DocumentHistory::takeSnapshot(const Document& currentDocument)
{
	if (!isCurrentDocumentLastOfHistory()) {
		m_documentHistory.erase(m_currentDocumentIt + 1, m_documentHistory.end());
	}

	constexpr int MaximumSnapshots = 100;
	if (m_documentHistory.size() == MaximumSnapshots) {
		m_documentHistory.erase(m_documentHistory.begin());
	}

	m_currentDocumentIt = m_documentHistory.insert(m_documentHistory.end(), currentDocument);
}

const Document& DocumentHistory::undo()
{
	if (!isCurrentDocumentFirstOfHistory()) {
		--m_currentDocumentIt;
	}

	return *m_currentDocumentIt;
}

const Document& DocumentHistory::redo()
{
	if (!isCurrentDocumentLastOfHistory()) {
		++m_currentDocumentIt;
	}

	return *m_currentDocumentIt;
}

}
