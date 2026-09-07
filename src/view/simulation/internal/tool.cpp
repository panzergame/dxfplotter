module;

#include <QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QGoochMaterial>
#include <Qt3DExtras/QCylinderMesh>

export module view.simulation.internal.tool;

import model.simulation;

export namespace view::simulation::internal
{

class Tool : public Qt3DCore::QEntity
{
private:
	Qt3DCore::QTransform* m_transform;
	Qt3DExtras::QGoochMaterial* m_material;
	const QVector3D m_halfHeight;

public:
	explicit Tool(Qt3DCore::QEntity* parent, float radius, float height);

	void setPosition(const model::Simulation::ToolPathPoint3D& position);
};

}

namespace view::simulation::internal
{

Tool::Tool(Qt3DCore::QEntity* parent, float radius, float height)
	: Qt3DCore::QEntity(parent)
	, m_transform(new Qt3DCore::QTransform(this))
	, m_material(new Qt3DExtras::QGoochMaterial(this))
	, m_halfHeight(0.0f, 0.0f, height / 2.0f)
{
	Qt3DExtras::QCylinderMesh* mesh = new Qt3DExtras::QCylinderMesh(this);
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

	static const QColor colorsByMoveType[] = { { 240, 248, 255 }, { 105, 105, 105 } };

	const QColor& colorForCurrentMoveType = colorsByMoveType[static_cast<int>(position.moveType)];
	m_material->setDiffuse(colorForCurrentMoveType);
}

}
