#pragma once

#include <model/document.h>

namespace model
{

class Document;

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
	const Document &undo();
	const Document &redo();
};

}
