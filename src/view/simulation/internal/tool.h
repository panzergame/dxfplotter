#pragma once

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QGoochMaterial>

#include <model/simulation.h>

namespace view::simulation::internal
{

class Tool : public Qt3DCore::QEntity
{
private:
	Qt3DCore::QTransform *m_transform;
	Qt3DExtras::QGoochMaterial *m_material;

public:
	explicit Tool(Qt3DCore::QEntity *parent, float radius, float height);

	void setPosition(const model::Simulation::ToolPathPoint3D& position);
};

}
