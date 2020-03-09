#include <exporter.h>
#include <postprocessor.h>

#include <iostream> // TODO

namespace Exporter::GCode
{

void Exporter::convertToGCode(const Model::Task &task)
{
	task.forEachPath([this](Model::Path *path){ convertToGCode(path); });
}

void Exporter::convertToGCode(Model::Path *path)
{
	PostProcessor processor(path->settings(), m_fileContent);
	convertToGCode(processor, path->polyline());
}

void Exporter::convertToGCode(PostProcessor &processor, const Geometry::Polyline &polyline)
{
	processor.fastMove(polyline.start());
	processor.laserOn();

	polyline.forEachBulge([this, &processor](const Geometry::Bulge &bulge){ convertToGCode(processor, bulge); });

	processor.laserOff();
}

void Exporter::convertToGCode(PostProcessor &processor, const Geometry::Bulge &bulge)
{
	if (bulge.isLine()) {
		processor.linearMove(bulge.end());
	}
	else {
		
	}
}

Exporter::Exporter(const Model::Task &task, const std::string &filename)
{
	convertToGCode(task);
	std::cout << m_fileContent.str();
}

}

