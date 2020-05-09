#include <exporter.h>
#include <pathpostprocessor.h>

#include <common/exception.h>

namespace Exporter::GCode
{

void Exporter::convertToGCode(const Model::Task *task)
{
	PostProcessor processor(m_format, m_file);

	task->forEachPath([this](Model::Path *path){ convertToGCode(path); });

	// Back to home
	processor.fastMove(QVector2D(0.0f, 0.0f));
}

void Exporter::convertToGCode(const Model::Path *path)
{
	const Model::PathSettings &settings = path->settings();
	PathPostProcessor processor(settings, m_format, m_file);

	const Geometry::Polyline::List polylines = path->finalPolylines();

	for (const Geometry::Polyline &polyline : polylines) {
		processor.fastMove(polyline.start());
		processor.toolOn();

		// Repeat for each passes
		for (int i = 0, passes = settings.passes(); i < passes; ++i) {
			convertToGCode(processor, polyline);
		}

		processor.toolOff();
	}
}

void Exporter::convertToGCode(PathPostProcessor &processor, const Geometry::Polyline &polyline)
{
	polyline.forEachBulge([this, &processor](const Geometry::Bulge &bulge){ convertToGCode(processor, bulge); });
}

void Exporter::convertToGCode(PathPostProcessor &processor, const Geometry::Bulge &bulge)
{
	if (bulge.isLine()) {
		processor.linearMove(bulge.end());
	}
	else {
		const Geometry::Circle circle = bulge.toCircle();
		// Relative center to start
		const QVector2D relativeCenter = circle.center() - bulge.start();
		switch (circle.orientation()) {
			case Geometry::Orientation::CW:
			{
				processor.cwArc(relativeCenter, bulge.end());
				break;
			}
			case Geometry::Orientation::CCW:
			{
				processor.ccwArc(relativeCenter, bulge.end());
				break;
			}
		}
	}
}

Exporter::Exporter(const Model::Task *task, const Format &format, const std::string &filename)
	:m_file(filename),
	m_format(format)
{
	if (!m_file) {
		throw Common::FileException();
	}

	convertToGCode(task);
}

}

