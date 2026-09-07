module;

#include <QObject>

export module model.document;

import config.config;
import model.path;
import model.task;
import common.aggregable;
export namespace model
{

class Document : public common::Aggregable<Document>
{
private:
	Task::UPtr m_task;
	const config::Tools::Tool* m_toolConfig;
	const config::Profiles::Profile* m_profileConfig;

public:
	explicit Document(Task::UPtr&& task, const config::Tools::Tool& toolConfig,
					  const config::Profiles::Profile& profileConfig);
	Document() = default;
	explicit Document(const Document& other);

	Document& operator=(Document&&) = default;

	Task& task();
	const Task& task() const;

	const config::Tools::Tool& toolConfig() const;
	const config::Profiles::Profile& profileConfig() const;
	void setToolConfig(const config::Tools::Tool& tool);
	void setProfileConfig(const config::Profiles::Profile& profile);
};

}

namespace model
{

Document::Document(Task::UPtr&& task, const config::Tools::Tool& toolConfig,
				   const config::Profiles::Profile& profileConfig)
	: m_task(std::move(task))
	, m_toolConfig(&toolConfig)
	, m_profileConfig(&profileConfig)
{
}

Document::Document(const Document& other)
	: m_task(std::make_unique<Task>(other.task()))
	, m_toolConfig(&other.toolConfig())
	, m_profileConfig(&other.profileConfig())
{
}

Task& Document::task()
{
	return *m_task;
}

const Task& Document::task() const
{
	return *m_task;
}

const config::Tools::Tool& Document::toolConfig() const
{
	return *m_toolConfig;
}

const config::Profiles::Profile& Document::profileConfig() const
{
	return *m_profileConfig;
}

void Document::setToolConfig(const config::Tools::Tool& tool)
{
	m_toolConfig = &tool;
}

void Document::setProfileConfig(const config::Profiles::Profile& profile)
{
	m_profileConfig = &profile;
}

}
