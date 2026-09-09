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
					  const config::Profiles::Profile& profileConfig)
		: m_task(std::move(task))
		, m_toolConfig(&toolConfig)
		, m_profileConfig(&profileConfig)
	{
	}

	Document() = default;
	explicit Document(const Document& other)
		: m_task(std::make_unique<Task>(other.task()))
		, m_toolConfig(&other.toolConfig())
		, m_profileConfig(&other.profileConfig())
	{
	}

	Document& operator=(Document&&) = default;

	Task& task() { return *m_task; }

	const Task& task() const { return *m_task; }

	const config::Tools::Tool& toolConfig() const { return *m_toolConfig; }

	const config::Profiles::Profile& profileConfig() const { return *m_profileConfig; }

	void setToolConfig(const config::Tools::Tool& tool) { m_toolConfig = &tool; }

	void setProfileConfig(const config::Profiles::Profile& profile) { m_profileConfig = &profile; }
};

}
