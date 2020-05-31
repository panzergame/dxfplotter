#pragma once

#include <vector>
#include <array>

namespace Common
{

template <class Item>
class Aggregable
{
public:
	using List = std::vector<Item>;
	using ListPtr = std::vector<Item *>;

	template <size_t Size>
	using Array = std::array<Item, Size>;
	template <size_t Size>
	using ArrayPtr = std::array<Item *, Size>;

	using Pair = Array<2>;
	using PairPtr = ArrayPtr<2>;
};

}
