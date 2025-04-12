#pragma once
#include "Niflect/Base/Memory.h"
#include <unordered_set>
#include <functional>

namespace Niflect
{
	template <typename TKey, typename THash, typename TCompare, typename TAllocator>
	using TSTLUnorderedSet = std::unordered_set<TKey, THash, TCompare, TAllocator>;

	template <typename TKey, typename THash = std::hash<TKey>, typename TCompare = std::equal_to<TKey> >
	using TUnorderedSet = TSTLUnorderedSet<TKey, THash, TCompare, THeapAllocator<TKey> >;
}