#pragma once

namespace Exporter::Dxfplot
{

template <class Entity>
struct Access;

}

template <class Archive, class Entity>
void save(Archive &archive, const Entity &entity)
{
	Exporter::Dxfplot::Access<Entity> access;
	access(archive, entity);
}
