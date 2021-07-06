#pragma once

#include <cereal/cereal.hpp>

namespace Exporter::Dxfplot
{

template <class Entity>
struct Access;

}

namespace cereal
{

template <class Archive, class Entity>
auto save(Archive &archive, const Entity &entity) -> decltype(Exporter::Dxfplot::Access<Entity>().save(archive, entity), void())
{
	Exporter::Dxfplot::Access<Entity> access;
	access.save(archive, entity);
}

template <class Archive, class Entity>
auto load(const Archive &archive, Entity &entity) -> decltype(Exporter::Dxfplot::Access<Entity>().load(archive, entity), void())
{
	Exporter::Dxfplot::Access<Entity> access;
	access.load(archive, entity);
}

template <class Archive, class Entity>
auto serialize(Archive &archive, Entity &entity) -> decltype(Exporter::Dxfplot::Access<Entity>().serialize(archive, entity), void())
{
	Exporter::Dxfplot::Access<Entity> access;
	access.serialize(archive, entity);
}

/*template <class Archive, class Entity>
auto load(const Archive &archive, Entity &entity) -> decltype(Exporter::Dxfplot::Access<Entity>(), void())
{
	Exporter::Dxfplot::Access<Entity> access;
	access.load(archive, entity);
}

template <class Archive, class Entity>
auto serialize(Archive &archive, const Entity &entity) -> decltype(Exporter::Dxfplot::Access<Entity>(), void())
{
	Exporter::Dxfplot::Access<Entity> access;
	access.serialize(archive, entity);
}*/

}
