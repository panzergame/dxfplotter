#pragma once

#include <gtest/gtest.h>

#include <yaml-cpp/yaml.h>

import config.config;
import geometry.polyline;
import model.document;
import model.pathsettings;
import model.task;

class ExporterFixture : public ::testing::Test
{
protected:
	const config::Tools::Tool m_tool{"tool", YAML::Node()};
	const config::Profiles::Profile::Gcode m_gcode{"gcode", YAML::Node()};
	const config::Profiles::Profile m_profile{"profile", YAML::Node()};
	const model::PathSettings m_settings{10, 10, 10, 0.1};
	model::Task *m_task;
	model::Document::UPtr m_document;
	std::ostringstream m_output;

	void createTaskFromPolyline(geometry::Polyline &&polyline);
};

