#pragma once

#include <cereal/cereal.hpp>

namespace serializer
{

template <class Entity>
struct Access;

}

namespace cereal
{

template <class Archive, class Entity>
auto save(Archive &archive, const Entity &entity) -> decltype(serializer::Access<Entity>().save(archive, entity), void())
{
	serializer::Access<Entity> access;
	access.save(archive, entity);
}

template <class Archive, class Entity>
auto load(Archive &archive, Entity &entity) -> decltype(serializer::Access<Entity>().load(archive, entity), void())
{
	serializer::Access<Entity> access;
	access.load(archive, entity);
}

template <class Archive, class Entity>
auto serialize(Archive &archive, Entity &entity) -> decltype(serializer::Access<Entity>().serialize(archive, entity), void())
{
	serializer::Access<Entity> access;
	access.serialize(archive, entity);
}

}
