module;

#include <array>
#include <cstddef>
#include <memory>
#include <set>
#include <vector>

export module common.aggregable;

export namespace common
{

template<class Item>
class Aggregable
{
public:
	using UPtr = std::unique_ptr<Item>;

	using List = std::vector<Item>;
	using ListPtr = std::vector<Item*>;
	using ListCPtr = std::vector<const Item*>;
	using ListUPtr = std::vector<UPtr>;

	template<std::size_t Size>
	using Array = std::array<Item, Size>;
	template<std::size_t Size>
	using ArrayPtr = std::array<Item*, Size>;

	using SetPtr = std::set<Item*>;

	using Pair = Array<2>;
	using PairPtr = ArrayPtr<2>;
};

}
