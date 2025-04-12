#pragma once
#include "Niflect/Base/Memory.h"
#include <unordered_map>
#include <functional>

namespace Niflect
{
	template <typename TKey, typename TValue, typename THash, typename TCompare, typename TAllocator>
	using TSTLUnorderedMap = std::unordered_map<TKey, TValue, THash, TCompare, TAllocator>;

	template <typename TKey, typename TValue, typename THash = std::hash<TKey>, typename TCompare = std::equal_to<TKey> >
	using TUnorderedMap = TSTLUnorderedMap<TKey, TValue, THash, TCompare, THeapAllocator<std::pair<const TKey, TValue> > >;
}