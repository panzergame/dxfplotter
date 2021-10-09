#pragma once

#include <gtest/gtest.h>

#include <model/document.h>

class ExporterFixture : public ::testing::Test
{
protected:
	const Config::Tools::Tool m_tool{"tool", YAML::Node()};
	const Config::Profiles::Profile::Gcode m_gcode{"gcode", YAML::Node()};
	const Config::Profiles::Profile m_profile{"profile", YAML::Node()};
	const Model::PathSettings m_settings{10, 10, 10, 0.1};
	Model::Task *m_task;
	Model::Document::UPtr m_document;
	std::ostringstream m_output;

	void createTaskFromPolyline(Geometry::Polyline &&polyline);
};

