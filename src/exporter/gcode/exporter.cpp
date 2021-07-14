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
	const Geometry::CuttingDirection cuttingDirection = path.cuttingDirection();
	PathPostProcessor processor(settings, m_tool, m_gcode, m_output);

	const Geometry::Polyline::List polylines = path.finalPolylines();

	// Depth to be cut
	const float depth = settings.depth();

	for (const Geometry::Polyline &polyline : polylines) {
		// Move to polyline begining
		processor.fastPlaneMove(polyline.start());
		processor.preCut();

		convertToGCode(processor, polyline, depth, cuttingDirection);

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
	const bool m_closed;
	const bool m_cuttingBackward;
	const Geometry::Polyline& m_polyline;
	const Geometry::Polyline m_polylineInverse;

	bool needPolylineInverse() const
	{
		return (!m_closed || m_cuttingBackward);
	}

	const Geometry::Polyline &polylineForward() const
	{
		return (m_cuttingBackward) ? m_polylineInverse : m_polyline;
	}

	const Geometry::Polyline &polylineBackward() const
	{
		return (m_cuttingBackward) ? m_polyline : m_polylineInverse;
	}

public:
	explicit PassesIterator(const Geometry::Polyline &polyline, Geometry::CuttingDirection direction)
		:m_odd(true),
		m_closed(polyline.isClosed()),
		m_cuttingBackward(direction == Geometry::CuttingDirection::BACKWARD),
		m_polyline(polyline),
		m_polylineInverse(needPolylineInverse() ? m_polyline.inverse() : Geometry::Polyline())
	{
	}

	const Geometry::Polyline &operator*() const
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

void Exporter::convertToGCode(PathPostProcessor &processor, const Geometry::Polyline &polyline, float maxDepth, Geometry::CuttingDirection cuttingDirection)
{
	const float depthPerCut = m_tool.general().depthPerCut();

	PassesIterator iterator(polyline, cuttingDirection);
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
			default:
				break;
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

