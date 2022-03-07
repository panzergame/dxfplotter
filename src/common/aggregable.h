#pragma once

#include <vector>
#include <array>
#include <set>
#include <memory>

namespace common
{

template <class Item>
class Aggregable
{
public:
	using UPtr = std::unique_ptr<Item>;

	using List = std::vector<Item>;
	using ListPtr = std::vector<Item *>;
	using ListUPtr = std::vector<UPtr>;

	template <size_t Size>
	using Array = std::array<Item, Size>;
	template <size_t Size>
	using ArrayPtr = std::array<Item *, Size>;

	using SetPtr = std::set<Item *>;

	using Pair = Array<2>;
	using PairPtr = ArrayPtr<2>;
};

}
