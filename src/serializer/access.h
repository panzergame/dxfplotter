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
auto save(Archive &archive, const Entity &entity, std::uint32_t const version) -> decltype(serializer::Access<Entity>().save(archive, entity, version), void())
{
	serializer::Access<Entity> access;
	access.save(archive, entity, version);
}

template <class Archive, class Entity>
auto load(Archive &archive, Entity &entity, std::uint32_t const version) -> decltype(serializer::Access<Entity>().load(archive, entity, version), void())
{
	serializer::Access<Entity> access;
	access.load(archive, entity, version);
}

template <class Archive, class Entity>
auto serialize(Archive &archive, Entity &entity, std::uint32_t const version) -> decltype(serializer::Access<Entity>().serialize(archive, entity, version), void())
{
	serializer::Access<Entity> access;
	access.serialize(archive, entity, version);
}

}
