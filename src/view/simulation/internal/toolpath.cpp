#include <toolpath.h>

#include <QGeometry>
#include <QGeometryRenderer>
#include <QAttribute> 
#include <QBuffer> 

#include <Qt3DExtras/QPerVertexColorMaterial>

#include <QByteArray>
#include <QBuffer>

#include <QDebug> // TODO

namespace view::simulation::internal
{

struct PackedVector3D
{
	float x;
	float y;
	float z;

	PackedVector3D() = default;

	explicit PackedVector3D(const QVector3D& other)
		:x(other.x()),
		y(other.y()),
		z(other.z())
	{
	}
};

void ToolPath::createPolylineFromPoints(const model::Simulation::ToolPathPoint3D::List& points)
{
	static const uint32_t colorsByMoveType[] = {
		(uint32_t)QColor(255, 69, 0).value(),
		(uint32_t)QColor(72, 61, 139).value()
	};

	const int nbPoints = points.size();

	m_packedPoints = std::make_unique<PackedVector3D[]>(nbPoints);
	m_colors = std::make_unique<uint32_t[]>(nbPoints);
	m_indices = std::make_unique<uint32_t[]>(nbPoints);

	for (int i = 0; i < nbPoints; ++i) {
		const model::Simulation::ToolPathPoint3D &point = points[i];
		m_packedPoints[i] = PackedVector3D(point.position);
		m_colors[i] = colorsByMoveType[static_cast<int>(point.moveType)];
		m_indices[i] = i;
	}

    Qt3DCore::QGeometry *geometry = new Qt3DCore::QGeometry(this);

	const QByteArray vertexData = QByteArray::fromRawData((const char *)m_packedPoints.get(), sizeof(PackedVector3D) * nbPoints);
	Qt3DCore::QBuffer *vertexBuffer = new Qt3DCore::QBuffer(geometry);
	vertexBuffer->setData(vertexData);

	const QByteArray colorData = QByteArray::fromRawData((const char *)m_colors.get(), sizeof(uint32_t) * nbPoints);
	Qt3DCore::QBuffer *colorBuffer = new Qt3DCore::QBuffer(geometry);
	colorBuffer->setData(colorData);

	const QByteArray indexData = QByteArray::fromRawData((const char *)m_indices.get(), sizeof(uint32_t) * nbPoints);
	Qt3DCore::QBuffer *indexBuffer = new Qt3DCore::QBuffer(geometry);
	indexBuffer->setData(indexData);

	Qt3DCore::QAttribute *vertexAttribute = new Qt3DCore::QAttribute(vertexBuffer, Qt3DCore::QAttribute::defaultPositionAttributeName(), Qt3DCore::QAttribute::Float, 3, nbPoints);
	vertexAttribute->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);

	Qt3DCore::QAttribute *colorAttribute = new Qt3DCore::QAttribute(colorBuffer, Qt3DCore::QAttribute::defaultColorAttributeName(), Qt3DCore::QAttribute::UnsignedByte, 4, nbPoints);
	colorAttribute->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);

	Qt3DCore::QAttribute *indexAttribute = new Qt3DCore::QAttribute(indexBuffer, Qt3DCore::QAttribute::UnsignedInt, 3, nbPoints);
	indexAttribute->setAttributeType(Qt3DCore::QAttribute::IndexAttribute);

	geometry->addAttribute(vertexAttribute);
	geometry->addAttribute(colorAttribute);
	geometry->addAttribute(indexAttribute);

	Qt3DRender::QGeometryRenderer *renderer = new Qt3DRender::QGeometryRenderer(this);
	renderer->setGeometry(geometry);
	renderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::LineStrip);

	Qt3DExtras::QPerVertexColorMaterial *material = new Qt3DExtras::QPerVertexColorMaterial(this);

	addComponent(material);
	addComponent(renderer);
}

ToolPath::ToolPath(Qt3DCore::QEntity *parent, const model::Simulation::ToolPathPoint3D::List& points)
	:Qt3DCore::QEntity(parent)
{
	createPolylineFromPoints(points);
}

ToolPath::~ToolPath() = default;

const double (&ToolPath::boundingBox() const)[6]
{
	return m_boundingBox;
}


}
