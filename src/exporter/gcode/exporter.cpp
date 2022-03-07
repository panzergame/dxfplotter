#include <exporter.h>
#include <pathpostprocessor.h>

namespace exporter::gcode
{

void Exporter::convertToGCode(const model::Task &task, std::ostream &output) const
{
	PostProcessor processor(m_tool, m_profile.gcode(), output);

	// Retract tool before work piece
	processor.retractDepth();

	task.forEachPathInStack([this, &output](const model::Path &path){
		if (path.globallyVisible()) {
			convertToGCode(path, output);
		}
	});

	// Back to home
	processor.fastPlaneMove(QVector2D(0.0f, 0.0f));
}

void Exporter::convertToGCode(const model::Path &path, std::ostream &output) const
{
	const model::PathSettings &settings = path.settings();
	const geometry::CuttingDirection cuttingDirection = path.cuttingDirection() | m_profile.cut().direction();
	PathPostProcessor processor(settings, m_tool, m_profile.gcode(), output);

	const geometry::Polyline::List polylines = path.finalPolylines();

	// Depth to be cut
	const float depth = settings.depth();

	for (const geometry::Polyline &polyline : polylines) {
		convertToGCode(processor, polyline, depth, cuttingDirection);
	}
}

// Return next polyline to convert
class PassesIterator
{
private:
	bool m_odd = true;
	const bool m_closed;
	const bool m_cuttingBackward;
	const geometry::Polyline& m_polyline;
	const geometry::Polyline m_polylineInverse;

	bool needPolylineInverse() const
	{
		return (!m_closed || m_cuttingBackward);
	}

	const geometry::Polyline &polylineForward() const
	{
		return m_cuttingBackward ? m_polylineInverse : m_polyline;
	}

	const geometry::Polyline &polylineBackward() const
	{
		return m_cuttingBackward ? m_polyline : m_polylineInverse;
	}

public:
	explicit PassesIterator(const geometry::Polyline &polyline, geometry::CuttingDirection direction)
		:m_closed(polyline.isClosed()),
		m_cuttingBackward(direction == geometry::CuttingDirection::BACKWARD),
		m_polyline(polyline),
		m_polylineInverse(needPolylineInverse() ? m_polyline.inverse() : geometry::Polyline())
	{
	}

	const geometry::Polyline &operator*() const
	{
		if (m_closed || m_odd) {
			return polylineForward();
		}
		return polylineBackward();
	}

	PassesIterator &operator++()
	{
		m_odd = !m_odd;

		return *this;
	}
};

void Exporter::convertToGCode(PathPostProcessor &processor, const geometry::Polyline &polyline, float maxDepth, geometry::CuttingDirection cuttingDirection) const
{
	const float depthPerCut = m_tool.general().depthPerCut();

	PassesIterator iterator(polyline, cuttingDirection);

	// Move to polyline beginning
	processor.fastPlaneMove((*iterator).start());
	processor.preCut();

	for (float depth = 0.0f; depth < maxDepth + depthPerCut; depth += depthPerCut, ++iterator) {
		const float boundDepth = std::fminf(depth, maxDepth);
		processor.depthLinearMove(-boundDepth);

		convertToGCode(processor, *iterator);
	}

	// Retract tool for further operations
	processor.retractDepth();
	processor.postCut();
}

void Exporter::convertToGCode(PathPostProcessor &processor, const geometry::Polyline &polyline) const
{
	polyline.forEachBulge([this, &processor](const geometry::Bulge &bulge){ convertToGCode(processor, bulge); });
}

void Exporter::convertToGCode(PathPostProcessor &processor, const geometry::Bulge &bulge) const
{
	if (bulge.isLine()) {
		processor.planeLinearMove(bulge.end());
	}
	else {
		const geometry::Circle circle = bulge.toCircle();
		// Relative center to start
		const QVector2D relativeCenter = circle.center() - bulge.start();
		switch (circle.orientation()) {
			case geometry::Orientation::CW:
				processor.cwArcMove(relativeCenter, bulge.end());
				break;
			case geometry::Orientation::CCW:
				processor.ccwArcMove(relativeCenter, bulge.end());
				break;
			default:
				break;
		}
	}
}

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

	convertToGCode(document.task(), output);
}


}

