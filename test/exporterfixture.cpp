#include <exporterfixture.h>

void ExporterFixture::createTaskFromPolyline(Geometry::Polyline &&polyline)
{
	Model::Path::UPtr path = std::make_unique<Model::Path>(std::move(polyline), "", m_settings);

	Model::Path::ListUPtr paths;
	paths.push_back(std::move(path));

	Model::Layer::UPtr layer = std::make_unique<Model::Layer>("layer", std::move(paths));

	Model::Layer::ListUPtr layers;
	layers.push_back(std::move(layer));
	Model::Task::UPtr task = std::make_unique<Model::Task>(std::move(layers));
	m_document = std::make_unique<Model::Document>(std::move(task), m_tool, m_profile);
	m_task = &m_document->task();
}
