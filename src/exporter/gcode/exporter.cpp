#include <exporter.h>
#include <metadata.h>
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

class RendererVisitor : private PostProcessor
{
public:
	using PostProcessor::PostProcessor;

	void start(const QVector2D& from, float safetyDepth)
	{
		retractDepth(safetyDepth);
	}

	void end(const QVector2D& to, float safetyDepth)
	{
		fastPlaneMove(to);
	}

	void startOperation(const QVector2D& to, float intensity)
	{
		// Move to polyline beginning and start tooling
		fastPlaneMove(to);
		preCut(intensity);
	}

	void endOperation(float safetyDepth)
	{
		// Retract tool for further operations
		retractDepth(safetyDepth);
		postCut();
	}

	void processPathAtDepth(const geometry::Polyline& polyline, float depth, float planeFeedRate, float depthFeedRate)
	{
		depthLinearMove(depth, depthFeedRate);

		polyline.forEachBulge([this, planeFeedRate](const geometry::Bulge &bulge){
			processBulge(bulge, planeFeedRate);
		});
	}
};


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

	const config::Profiles::Profile::Gcode& gcode = m_profile.gcode();

	if (m_options & ExportMetadata) {
		const Metadata metadata(document, gcode, m_tool.general().retractDepth());
		output << metadata.toComment();
	}

	RendererVisitor visitor(gcode, output);
	renderer::Renderer renderer(m_tool, m_profile, visitor);
	renderer.render(document);
}


}

