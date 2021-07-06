#pragma once

#include <exporter/dxfplot/access.h>
#include <exporter/dxfplot/layer.h>

#include <cereal/cereal.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

#include <utility>
#include <model/task.h>

namespace Exporter::Dxfplot
{

using IndexStackList = std::vector<int>;

int findPathIndex(const Model::Path::ListPtr& paths, const Model::Path *wantedPath)
{
	const Model::Path::ListPtr::const_iterator it = std::find(paths.cbegin(), paths.cend(), wantedPath);

	return std::distance(paths.cbegin(), it);
}

IndexStackList convertPathStackToIndexStack(const Model::Path::ListPtr& paths,
		const Model::Path::ListPtr& stack)
{
	IndexStackList indexStack(stack.size());
	std::transform(stack.begin(), stack.end(), indexStack.begin(),
		[&paths](const Model::Path *path){
			return findPathIndex(paths, path);
		}
	);

	return indexStack;
}

template<>
struct Access<Model::Task>
{
	template <class Archive>
	void save(Archive &archive, const Model::Task &task) const
	{
		archive(cereal::make_nvp("layers", task.m_layers));

		const IndexStackList indexStack = convertPathStackToIndexStack(task.m_paths, task.m_stack);
		archive(cereal::make_nvp("stack", indexStack));
	}
};

}
