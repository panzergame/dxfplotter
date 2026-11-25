module;

#include <cmath>
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
	const float m_startDepth;
	const float m_retractDepth;
	const float m_alignmentDepth;
	Visitor& m_visitor;

	void render(const model::Task& task) const
	{
		const QVector2D home(0.0f, 0.0f);

		// Retract tool before work piece
		m_visitor.start(home, m_retractDepth);

		task.forEachPathInStack([this](const model::Path& path) {
			if (path.globallyVisible()) {
				render(path);
			}
		});

		// Back to home
		m_visitor.end(home, m_retractDepth);
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

		/* An entry only conditions the tool for the polyline start, which a closed polyline ends at. An open
		 * polyline ends elsewhere, so each pass re-enters instead of travelling backward.
		 */
		const bool reenter = entry && !toolPolyline.polyline.isClosed();

		PassesIterator iterator(toolPolyline.polyline, cuttingDirection, !reenter);

		m_visitor.startOperation(entry ? entry->start() : (*iterator).start(), intensity);

		// Alignment depth is a fixed blade setting, independent of the passes and their offset.
		const auto processEntry = [&](float depth) {
			m_visitor.processPathAtDepth(*entry, -depth, planeFeedRate, depthFeedRate);
		};

		if (m_hasDepth) {
			if (entry) {
				processEntry(m_alignmentDepth);
			}
			const float depthShift = settings.depth();
			const float depthOffset = settings.depthOffset();
			const float startDepth = depthOffset + m_startDepth;
			const float finalDepth = depthOffset + depthShift;

			// Epsilon prevents a quotient like 3.0000002 from adding a duplicated pass at max depth.
			const int passCount = int(std::ceil((depthShift - m_startDepth) / m_depthPerCut - 1e-4f)) + 1;

			for (int pass = 0; pass < passCount; ++pass) {
				const float depth = std::fminf(startDepth + pass * m_depthPerCut, finalDepth);

				if (reenter && pass > 0) {
					m_visitor.endOperation(m_retractDepth);
					m_visitor.startOperation(entry->start(), intensity);
					processEntry(m_alignmentDepth);
				}

				m_visitor.processPathAtDepth(*iterator, -depth, planeFeedRate, depthFeedRate);

				++iterator;
			}
		} else {
			if (entry) {
				processEntry(0.0f);
			}
			m_visitor.processPathAtDepth(*iterator, 0.0f, planeFeedRate, depthFeedRate);
		}
		m_visitor.endOperation(m_retractDepth);
	}

public:
	explicit Renderer(const config::Tools::Tool& tool, const config::Profiles::Profile& profile, Visitor& visitor)
		: m_tool(tool)
		, m_profile(profile)
		, m_hasDepth(m_tool.type() != config::ToolType::Laser)
		, m_depthPerCut(m_tool.general().depthPerCut())
		, m_startDepth(m_profile.cut().passAtZeroDepth() ? 0.0f : m_depthPerCut)
		, m_retractDepth(m_hasDepth ? m_tool.general().retractDepth() : 0.0f)
		, m_alignmentDepth(m_tool.dragKnife().alignmentDepth())
		, m_visitor(visitor)
	{
	}

	void render(const model::Document& document) { render(document.task()); }
};

}
