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
auto save(Archive &archive, const Entity &entity) -> decltype(Exporter::Dxfplot::Access<Entity>(), void())
{
	Exporter::Dxfplot::Access<Entity> access;
	access(archive, entity);
}

}
