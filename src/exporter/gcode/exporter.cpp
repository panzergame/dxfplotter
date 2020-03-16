#include <exporter.h>
#include <postprocessor.h>

#include <iostream> // TODO

namespace Exporter::GCode
{



void Exporter::convertToGCode(const Model::Task *task)
{
	task->forEachPath([this](Model::Path *path){ convertToGCode(path); });
}

void Exporter::convertToGCode(const Model::Path *path)
{
	PostProcessor processor(path->settings(), m_format, m_fileContent);
	convertToGCode(processor, path->polyline());
}

void Exporter::convertToGCode(PostProcessor &processor, const Geometry::Polyline &polyline)
{
	processor.fastMove(polyline.start());
	processor.toolOn();

	polyline.forEachBulge([this, &processor](const Geometry::Bulge &bulge){ convertToGCode(processor, bulge); });

	processor.toolOff();
}

void Exporter::convertToGCode(PostProcessor &processor, const Geometry::Bulge &bulge)
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
	:m_format(format)
{
	convertToGCode(task);
	std::cout << m_fileContent.str();
}

}

