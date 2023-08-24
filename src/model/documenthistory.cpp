#include <documenthistory.h>

#include <QDebug>

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
	:m_currentDocumentIt(m_documentHistory.insert(m_documentHistory.end(), initialDocument))
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

const Document &DocumentHistory::undo()
{
	if (!isCurrentDocumentFirstOfHistory()) {
		--m_currentDocumentIt;
	}

	return *m_currentDocumentIt;
}

const Document &DocumentHistory::redo()
{
	if (!isCurrentDocumentLastOfHistory()) {
		++m_currentDocumentIt;
	}

	return *m_currentDocumentIt;
}

}
