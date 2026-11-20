module;

#include <optional>
#include <QVector2D>

export module exporter.renderer.renderer;

import config.config;
import config.enums;
import exporter.renderer.passesiterator;
import geometry.tooloperation.tooloperation;
import geometry.utils;
import model.document;
import geometry.polyline;
import model.path;
import model.pathsettings;
import model.task;

export namespace exporter::renderer
{

template<class Visitor>
class Renderer
{
private:
	const config::Tools::Tool& m_tool;
	const config::Profiles::Profile& m_profile;
	const bool m_hasDepth;
	const float m_depthPerCut;
	const float m_maximumStartDepth;
	const float m_depthToRetract;
	const float m_alignmentDepth;
	Visitor& m_visitor;

	void render(const model::Task& task) const
	{
		const QVector2D home(0.0f, 0.0f);

		// Retract tool before work piece
		m_visitor.start(home, m_depthToRetract);

		task.forEachPathInStack([this](const model::Path& path) {
			if (path.globallyVisible()) {
				render(path);
			}
		});

		// Back to home
		m_visitor.end(home, m_depthToRetract);
	}

	void render(const model::Path& path) const
	{
		const model::PathSettings& settings = path.settings();
		const geometry::CuttingDirection cuttingDirection = path.cuttingDirection() | m_profile.cut().direction();

		const geometry::tooloperation::ToolPolyline::List polylines = path.finalPolylines();

		// Depth to be cut

		for (const geometry::tooloperation::ToolPolyline& toolPolyline : polylines) {
			render(toolPolyline, settings, cuttingDirection);
		}
	}

	void render(const geometry::tooloperation::ToolPolyline& toolPolyline, const model::PathSettings& settings,
				geometry::CuttingDirection cuttingDirection) const
	{
		const float intensity = settings.intensity();
		const float planeFeedRate = settings.planeFeedRate();
		const float depthFeedRate = settings.depthFeedRate();
		const std::optional<geometry::Polyline>& entry = toolPolyline.entry;

		PassesIterator iterator(toolPolyline.polyline, cuttingDirection);

		m_visitor.startOperation(entry ? entry->start() : (*iterator).start(), intensity);

		const auto processEntry = [&](float depth) {
			m_visitor.processPathAtDepth(*entry, -std::fminf(m_alignmentDepth, depth), planeFeedRate, depthFeedRate);
		};

		if (m_hasDepth) {
			/* An entry only conditions the tool for the polyline start, which a closed polyline ends at. An open
			 * polyline ends elsewhere, so each pass re-enters instead of travelling backward.
			 */
			const bool reenter = entry && !toolPolyline.polyline.isClosed();
			const float maxDepth = settings.depth();
			const float startDepth = std::min(m_maximumStartDepth, maxDepth);
			if (entry) {
				processEntry(startDepth);
			}
			for (float depth = startDepth; depth < maxDepth + m_depthPerCut; depth += m_depthPerCut) {
				const float boundDepth = std::fminf(depth, maxDepth);

				if (reenter && depth != startDepth) {
					m_visitor.endOperation(m_depthToRetract);
					m_visitor.startOperation(entry->start(), intensity);
					processEntry(boundDepth);
				}

				m_visitor.processPathAtDepth(*iterator, -boundDepth, planeFeedRate, depthFeedRate);

				if (!reenter) {
					++iterator;
				}
			}
		} else {
			if (entry) {
				processEntry(0.0f);
			}
			m_visitor.processPathAtDepth(*iterator, 0.0f, planeFeedRate, depthFeedRate);
		}
		m_visitor.endOperation(m_depthToRetract);
	}

public:
	explicit Renderer(const config::Tools::Tool& tool, const config::Profiles::Profile& profile, Visitor& visitor)
		: m_tool(tool)
		, m_profile(profile)
		, m_hasDepth(m_tool.type() != config::ToolType::Laser)
		, m_depthPerCut(m_tool.general().depthPerCut())
		, m_maximumStartDepth(m_profile.cut().passAtZeroDepth() ? 0.0f : m_depthPerCut)
		, m_depthToRetract(m_hasDepth ? m_tool.general().retractDepth() : 0.0f)
		, m_alignmentDepth(m_tool.dragKnife().alignmentDepth())
		, m_visitor(visitor)
	{
	}

	void render(const model::Document& document) { render(document.task()); }
};

}
