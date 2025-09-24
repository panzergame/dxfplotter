#pragma once

#include <vector>
#include <memory>
#include <algorithm>

namespace common
{

template <class Item, class ItemUPtr = std::unique_ptr<Item>>
std::vector<ItemUPtr> deepcopy(const std::vector<ItemUPtr> &other)
{
	std::vector<ItemUPtr> duplicated(other.size());
	std::transform(other.begin(), other.end(), duplicated.begin(), [](const ItemUPtr &item){
		return std::make_unique<Item>(*item);
	});

	return duplicated;
}

}
