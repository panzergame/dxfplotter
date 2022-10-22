#include <exporter.h>
#include <postprocessor.h>
#include <exporter/renderer/renderer.h>

namespace exporter::gcode
{

struct CommentLineStream
{
	std::ostream &m_output;

	explicit CommentLineStream(std::ostream &output, const std::string &prefix)
		:m_output(output)
	{
		m_output << "; " << prefix;
	}

	CommentLineStream(CommentLineStream &) = delete;

	~CommentLineStream()
	{
		m_output << "\n";
	}

	template <class Value>
	CommentLineStream& operator<<(const Value &value)
	{
		m_output << value;
		return *this;
	}
};

struct ConfigToCommentVisitor
{
	std::ostream &m_output;
	const std::string m_prefix;

	CommentLineStream commentLineStream() const
	{
		return CommentLineStream(m_output, m_prefix);
	}

	template <class ValueType>
	void operator()(const config::Property<ValueType> &property) const
	{
		commentLineStream() << property.name() << " = " << config::toSerializable((ValueType)property);
	}

	template <class Node>
	void operator()(const Node &node) const
	{
		commentLineStream() << node.name();
		node.visitChildren(ConfigToCommentVisitor{m_output, m_prefix + "  "});
	}
};

template <class Group>
void convertConfigNodeToComments(const Group &group, std::ostream &output)
{
	const ConfigToCommentVisitor visitor{output, ""};
	visitor(group);
}

Exporter::Exporter(const config::Tools::Tool& tool, const config::Profiles::Profile& profile, Options options)
	:m_tool(tool),
	m_profile(profile),
	m_options(options)
{
}

void Exporter::operator()(const model::Document &document, std::ostream &output) const
{
	if (m_options & ExportConfig) {
		convertConfigNodeToComments(m_tool, output);
		convertConfigNodeToComments(m_profile, output);
	}

	PostProcessor processor(m_profile.gcode(), output);
	renderer::Renderer renderer(m_tool, m_profile, processor);
	renderer.render(document);
}


}

