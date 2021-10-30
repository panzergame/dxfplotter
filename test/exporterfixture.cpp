#include <exporterfixture.h>

void ExporterFixture::createTaskFromPolyline(geometry::Polyline &&polyline)
{
	model::Path::UPtr path = std::make_unique<model::Path>(std::move(polyline), "", m_settings);

	model::Path::ListUPtr paths;
	paths.push_back(std::move(path));

	model::Layer::UPtr layer = std::make_unique<model::Layer>("layer", std::move(paths));

	model::Layer::ListUPtr layers;
	layers.push_back(std::move(layer));
	model::Task::UPtr task = std::make_unique<model::Task>(std::move(layers));
	m_document = std::make_unique<model::Document>(std::move(task), m_tool, m_profile);
	m_task = &m_document->task();
}
