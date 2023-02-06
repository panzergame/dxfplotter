#include <toolpath.h>

#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QAttribute> 
#include <Qt3DRender/QBuffer> 

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

    Qt3DRender::QGeometry *geometry = new Qt3DRender::QGeometry(this);

	const QByteArray vertexData = QByteArray::fromRawData((const char *)m_packedPoints.get(), sizeof(PackedVector3D) * nbPoints);
	Qt3DRender::QBuffer *vertexBuffer = new Qt3DRender::QBuffer(geometry);
	vertexBuffer->setData(vertexData);

	const QByteArray colorData = QByteArray::fromRawData((const char *)m_colors.get(), sizeof(uint32_t) * nbPoints);
	Qt3DRender::QBuffer *colorBuffer = new Qt3DRender::QBuffer(geometry);
	colorBuffer->setData(colorData);

	const QByteArray indexData = QByteArray::fromRawData((const char *)m_indices.get(), sizeof(uint32_t) * nbPoints);
	Qt3DRender::QBuffer *indexBuffer = new Qt3DRender::QBuffer(geometry);
	indexBuffer->setData(indexData);

	Qt3DRender::QAttribute *vertexAttribute = new Qt3DRender::QAttribute(vertexBuffer, Qt3DRender::QAttribute::defaultPositionAttributeName(), Qt3DRender::QAttribute::Float, 3, nbPoints);
	vertexAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);

	Qt3DRender::QAttribute *colorAttribute = new Qt3DRender::QAttribute(colorBuffer, Qt3DRender::QAttribute::defaultColorAttributeName(), Qt3DRender::QAttribute::UnsignedByte, 4, nbPoints);
	colorAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);

	Qt3DRender::QAttribute *indexAttribute = new Qt3DRender::QAttribute(indexBuffer, Qt3DRender::QAttribute::UnsignedInt, 3, nbPoints);
	indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);

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
