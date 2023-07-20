#pragma once

#include <Qt3DCore/QEntity>

#include <model/simulation.h>

namespace view::simulation::internal
{

struct PackedVector3D;

class ToolPath : public Qt3DCore::QEntity
{
private:
	double m_boundingBox[6];

	std::unique_ptr<PackedVector3D[]> m_packedPoints;
	std::unique_ptr<uint32_t[]> m_colors;
	std::unique_ptr<uint32_t[]> m_indices;

	void createPolylineFromPoints(const model::Simulation::ToolPathPoint3D::List &points);

public:
	ToolPath() = default;
	explicit ToolPath(Qt3DCore::QEntity *parent, const model::Simulation::ToolPathPoint3D::List &points);
	~ToolPath();

	const double (&boundingBox() const)[6];
};

}
