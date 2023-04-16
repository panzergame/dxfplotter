#include <tool.h>

#include <Qt3DExtras/QCylinderMesh>

namespace view::simulation::internal
{

Tool::Tool(Qt3DCore::QEntity *parent, float radius, float height)
	:Qt3DCore::QEntity(parent),
	m_transform(new Qt3DCore::QTransform(this)),
	m_material(new Qt3DExtras::QGoochMaterial(this)),
	m_halfHeight(0.0f, 0.0f, height / 2.0f)
{
	Qt3DExtras::QCylinderMesh *mesh = new Qt3DExtras::QCylinderMesh(this);
	mesh->setRadius(radius);
	mesh->setLength(height);

	m_transform->setRotationX(90);
	m_transform->setTranslation(m_halfHeight);

	addComponent(m_transform);
	addComponent(mesh);
	addComponent(m_material);
}

void Tool::setPosition(const model::Simulation::ToolPathPoint3D& position)
{
	m_transform->setTranslation(position.position + m_halfHeight);

	static const QColor colorsByMoveType[] = {
		{240, 248, 255},
		{105, 105, 105}
	};

	const QColor &colorForCurrentMoveType = colorsByMoveType[static_cast<int>(position.moveType)];
	m_material->setDiffuse(colorForCurrentMoveType);
}

}
