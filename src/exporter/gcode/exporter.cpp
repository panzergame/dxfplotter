#include <exporter.h>
#include <pathpostprocessor.h>

namespace Exporter::GCode
{

void Exporter::convertToGCode(const Model::Task &task)
{
	PostProcessor processor(m_tool, m_gcode, m_output);

	// Retract tool before work piece
	processor.retractDepth();

	task.forEachPathInStack([this](const Model::Path &path){
		if (path.globallyVisible()) {
			convertToGCode(path);
		}
	});

	// Back to home
	processor.fastPlaneMove(QVector2D(0.0f, 0.0f));
}

void Exporter::convertToGCode(const Model::Path &path)
{
	const Model::PathSettings &settings = path.settings();
	PathPostProcessor processor(settings, m_tool, m_gcode, m_output);

	const Geometry::Polyline::List polylines = path.finalPolylines();

	// Depth to be cut
	const float depth = settings.depth();

	for (const Geometry::Polyline &polyline : polylines) {
		// Move to polyline begining
		processor.fastPlaneMove(polyline.start());
		processor.preCut();

		convertToGCode(processor, polyline, depth);

		// Retract tool for further operations
		processor.retractDepth();
		processor.postCut();
	}
}

// Return next polyline to convert
class PassesIterator
{
private:
	bool m_odd;
	bool m_closed;
	const Geometry::Polyline& m_polyline;
	const Geometry::Polyline m_polylineInverse;

public:
	explicit PassesIterator(const Geometry::Polyline &polyline)
		:m_odd(true),
		m_closed(polyline.isClosed()),
		m_polyline(polyline),
		m_polylineInverse(!m_closed ? polyline.inverse() : Geometry::Polyline())
	{
	}

	const Geometry::Polyline &operator*() const
	{
		if (m_closed || m_odd) {
			return m_polyline;
		}
		return m_polylineInverse;
	}

	PassesIterator &operator++()
	{
		m_odd = !m_odd;

		return *this;
	}
};

void Exporter::convertToGCode(PathPostProcessor &processor, const Geometry::Polyline &polyline, float maxDepth)
{
	const float depthPerCut = m_tool.general().depthPerCut();

	PassesIterator iterator(polyline);
	for (float depth = depthPerCut; depth < maxDepth + depthPerCut; depth += depthPerCut, ++iterator) {
		const float boundDepth = std::fminf(depth, maxDepth);
		processor.depthLinearMove(-boundDepth);

		convertToGCode(processor, *iterator);
	}
}

void Exporter::convertToGCode(PathPostProcessor &processor, const Geometry::Polyline &polyline)
{
	polyline.forEachBulge([this, &processor](const Geometry::Bulge &bulge){ convertToGCode(processor, bulge); });
}

void Exporter::convertToGCode(PathPostProcessor &processor, const Geometry::Bulge &bulge)
{
	if (bulge.isLine()) {
		processor.planeLinearMove(bulge.end());
	}
	else {
		const Geometry::Circle circle = bulge.toCircle();
		// Relative center to start
		const QVector2D relativeCenter = circle.center() - bulge.start();
		switch (circle.orientation()) {
			case Geometry::Orientation::CW:
			{
				processor.cwArcMove(relativeCenter, bulge.end());
				break;
			}
			case Geometry::Orientation::CCW:
			{
				processor.ccwArcMove(relativeCenter, bulge.end());
				break;
			}
		}
	}
}

Exporter::Exporter(const Model::Task &task, const Config::Tools::Tool& tool, const Config::Profiles::Profile::Gcode& gcode, std::ostream &output)
	:m_output(output),
	m_tool(tool),
	m_gcode(gcode)
{
	convertToGCode(task);
}

}

