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
		m_visitor.retractDepth(m_depthToRetract);

		task.forEachPathInStack([this](const model::Path &path){
			if (path.globallyVisible()) {
				render(path);
			}
		});

		// Back to home
		m_visitor.fastPlaneMove(QVector2D(0.0f, 0.0f));
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
		const float depthFeedRate = settings.depthFeedRate();

		PassesIterator iterator(polyline, cuttingDirection);

		// Move to polyline beginning
		m_visitor.fastPlaneMove((*iterator).start());
		m_visitor.preCut(intensity);

		for (float depth = 0.0f; depth < maxDepth + m_depthPerCut; depth += m_depthPerCut, ++iterator) {
			const float boundDepth = std::fminf(depth, maxDepth);
			m_visitor.depthLinearMove(-boundDepth, depthFeedRate, intensity);

			render(*iterator, settings);
		}

		// Retract tool for further operations
		m_visitor.retractDepth(m_depthToRetract);
		m_visitor.postCut();
	}

	void render(const geometry::Polyline &polyline, const model::PathSettings &settings) const
	{
		polyline.forEachBulge([this, &settings](const geometry::Bulge &bulge){ render(bulge, settings); });
	}

	void render(const geometry::Bulge &bulge, const model::PathSettings &settings) const
	{
		const float intensity = settings.intensity();
		const float planeFeedRate = settings.planeFeedRate();

		if (bulge.isLine()) {
			m_visitor.planeLinearMove(bulge.end(), planeFeedRate, intensity);
		}
		else {
			const geometry::Circle circle = bulge.toCircle();
			// Relative center to start
			const QVector2D relativeCenter = circle.center() - bulge.start();
			switch (circle.orientation()) {
				case geometry::Orientation::CW:
					m_visitor.cwArcMove(relativeCenter, bulge.end(), planeFeedRate, intensity);
					break;
				case geometry::Orientation::CCW:
					m_visitor.ccwArcMove(relativeCenter, bulge.end(), planeFeedRate, intensity);
					break;
				default:
					break;
			}
		}
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
