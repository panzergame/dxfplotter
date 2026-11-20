module;

#include <Qt3DCore/QEntity>
#include <QGeometry>
#include <QGeometryRenderer>
#include <QAttribute>
#include <QBuffer>
#include <Qt3DExtras/QPerVertexColorMaterial>
#include <QByteArray>
#include <QDebug>

export module view.simulation.internal.toolpath;

import model.simulation;

export namespace view::simulation::internal
{

struct PackedVector3D
{
	float x;
	float y;
	float z;

	PackedVector3D() = default;

	explicit PackedVector3D(const QVector3D& other)
		: x(other.x())
		, y(other.y())
		, z(other.z())
	{
	}
};

class ToolPath : public Qt3DCore::QEntity
{
private:
	void createPolylineFromPoints(const model::Simulation::ToolPathPoint3D::List& points)
	{
		static const uint32_t colorsByMoveType[]
			= { (uint32_t)QColor(255, 69, 0).value(), (uint32_t)QColor(72, 61, 139).value() };

		const int nbPoints = points.size();

		QByteArray packedPointsArray(sizeof(PackedVector3D) * nbPoints, Qt::Uninitialized);
		QByteArray colorsArray(sizeof(uint32_t) * nbPoints, Qt::Uninitialized);
		QByteArray indicesArray(sizeof(uint32_t) * nbPoints, Qt::Uninitialized);
		PackedVector3D* const packedPoints = std::bit_cast<PackedVector3D*>(packedPointsArray.data());
		uint32_t* const colors = std::bit_cast<uint32_t*>(colorsArray.data());
		uint32_t* const indices = std::bit_cast<uint32_t*>(indicesArray.data());

		for (int i = 0; i < nbPoints; ++i) {
			const model::Simulation::ToolPathPoint3D& point = points[i];
			packedPoints[i] = PackedVector3D(point.position);
			colors[i] = colorsByMoveType[static_cast<int>(point.moveType)];
			indices[i] = i;
		}

		Qt3DCore::QGeometry* geometry = new Qt3DCore::QGeometry(this);

		Qt3DCore::QBuffer* vertexBuffer = new Qt3DCore::QBuffer(geometry);
		vertexBuffer->setData(packedPointsArray);
		Qt3DCore::QBuffer* colorBuffer = new Qt3DCore::QBuffer(geometry);
		colorBuffer->setData(colorsArray);
		Qt3DCore::QBuffer* indexBuffer = new Qt3DCore::QBuffer(geometry);
		indexBuffer->setData(indicesArray);

		Qt3DCore::QAttribute* vertexAttribute
			= new Qt3DCore::QAttribute(vertexBuffer, Qt3DCore::QAttribute::defaultPositionAttributeName(),
									   Qt3DCore::QAttribute::Float, 3, nbPoints);
		vertexAttribute->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);

		Qt3DCore::QAttribute* colorAttribute
			= new Qt3DCore::QAttribute(colorBuffer, Qt3DCore::QAttribute::defaultColorAttributeName(),
									   Qt3DCore::QAttribute::UnsignedByte, 4, nbPoints);
		colorAttribute->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);

		Qt3DCore::QAttribute* indexAttribute
			= new Qt3DCore::QAttribute(indexBuffer, Qt3DCore::QAttribute::UnsignedInt, 1, nbPoints);
		indexAttribute->setAttributeType(Qt3DCore::QAttribute::IndexAttribute);

		geometry->addAttribute(vertexAttribute);
		geometry->addAttribute(colorAttribute);
		geometry->addAttribute(indexAttribute);

		Qt3DRender::QGeometryRenderer* renderer = new Qt3DRender::QGeometryRenderer(this);
		renderer->setGeometry(geometry);
		renderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::LineStrip);

		Qt3DExtras::QPerVertexColorMaterial* material = new Qt3DExtras::QPerVertexColorMaterial(this);

		addComponent(material);
		addComponent(renderer);
	}

public:
	ToolPath() = default;
	explicit ToolPath(Qt3DCore::QEntity* parent, const model::Simulation::ToolPathPoint3D::List& points)
		: Qt3DCore::QEntity(parent)
	{
		createPolylineFromPoints(points);
	}

	~ToolPath() = default;
};

}
