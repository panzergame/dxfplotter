#pragma once

#include <model/document.h>
#include <exporter/renderer/passesiterator.h>

namespace exporter::renderer
{

template <class Visitor>
class Renderer
{
private:
	const config::Tools::Tool &m_tool;
	const config::Profiles::Profile &m_profile;
	const float m_depthPerCut;
	const float m_depthToRetract;
	Visitor &m_visitor;

	void render(const model::Task &task) const
	{
		// Retract tool before work piece
		m_visitor.start(m_depthToRetract);

		task.forEachPathInStack([this](const model::Path &path){
			if (path.globallyVisible()) {
				render(path);
			}
		});

		// Back to home
		m_visitor.end(QVector2D(0.0f, 0.0f));
	}

	void render(const model::Path &path) const
	{
		const model::PathSettings &settings = path.settings();
		const geometry::CuttingDirection cuttingDirection = path.cuttingDirection() | m_profile.cut().direction();

		const geometry::Polyline::List polylines = path.finalPolylines();

		// Depth to be cut

		for (const geometry::Polyline &polyline : polylines) {
			render(polyline, settings, cuttingDirection);
		}
	}

	void render(const geometry::Polyline &polyline, const model::PathSettings &settings, geometry::CuttingDirection cuttingDirection) const
	{
		const float maxDepth = settings.depth();
		const float intensity = settings.intensity();
		const float planeFeedRate = settings.planeFeedRate();
		const float depthFeedRate = settings.depthFeedRate();

		PassesIterator iterator(polyline, cuttingDirection);

		m_visitor.startOperation((*iterator).start(), intensity);

		for (float depth = 0.0f; depth < maxDepth + m_depthPerCut; depth += m_depthPerCut, ++iterator) {
			const float boundDepth = std::fminf(depth, maxDepth);

			m_visitor.processPathAtDepth(*iterator, -boundDepth, planeFeedRate, depthFeedRate);
		}

		m_visitor.endOperation(m_depthToRetract);
	}

public:
	explicit Renderer(const config::Tools::Tool& tool, const config::Profiles::Profile& profile, Visitor& visitor)
		:m_tool(tool),
		m_profile(profile),
		m_depthPerCut(m_tool.general().depthPerCut()),
		m_depthToRetract(m_tool.general().retractDepth()),
		m_visitor(visitor)
	{
	}

	void render(const model::Document &document)
	{
		render(document.task());
	}
};

}
