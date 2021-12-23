#pragma once

#include <serializer/access.h>
#include <serializer/layer.h>

#include <cereal/cereal.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

#include <utility>
#include <model/task.h>

#include <QDebug>

namespace serializer
{

using IndexStackList = std::vector<int>;

inline int findPathIndex(const model::Path::ListPtr& paths, const model::Path *wantedPath)
{
	const model::Path::ListPtr::const_iterator it = std::find(paths.cbegin(), paths.cend(), wantedPath);

	return std::distance(paths.cbegin(), it);
}

inline IndexStackList convertPathStackToIndexStack(const model::Path::ListPtr& paths,
		const model::Path::ListPtr& stack)
{
	IndexStackList indexStack(stack.size());
	std::transform(stack.begin(), stack.end(), indexStack.begin(),
		[&paths](const model::Path *path){
			return findPathIndex(paths, path);
		}
	);

	return indexStack;
}

inline model::Path::ListPtr convertIndexStackToPathStack(const model::Path::ListPtr& paths, const IndexStackList &indexStack)
{
	model::Path::ListPtr stack(indexStack.size());
	std::transform(indexStack.begin(), indexStack.end(), stack.begin(), [&paths](int index){
		return paths[index];
	});

	return stack;
}

template<>
struct Access<model::Task>
{
	template <class Archive>
	void save(Archive &archive, const model::Task &task, std::uint32_t const version) const
	{
		archive(cereal::make_nvp("layers", task.m_layers));

		const IndexStackList indexStack = convertPathStackToIndexStack(task.m_paths, task.m_stack);
		archive(cereal::make_nvp("stack", indexStack));
	}

	template <class Archive>
	void load(Archive &archive, model::Task &task, std::uint32_t const version) const
	{
		model::Layer::ListUPtr layers;
		archive(cereal::make_nvp("layers", task.m_layers));

		model::Path::ListPtr paths;
		for (const model::Layer::UPtr &layer : task.m_layers) {
			layer->forEachChild([&paths](model::Path &path){ paths.push_back(&path); });
		}

		IndexStackList indexStack;
		archive(cereal::make_nvp("stack", indexStack));
		task.m_stack = convertIndexStackToPathStack(paths, indexStack);

		task.initPathsFromLayers();
	}
};

}
