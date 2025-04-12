#pragma once
#include "Niflect/Base/Memory.h"
#include <map>
#include <functional>

namespace Niflect
{
	template <typename TKey, typename TValue, typename TCompare, typename TAllocator>
	using TSTLMap = std::map<TKey, TValue, TCompare, TAllocator>;
	template <typename TKey, typename TValue, typename TCompare = std::less<TKey> >
	using TMap = TSTLMap<TKey, TValue, TCompare, THeapAllocator<std::pair<const TKey, TValue> > >;
}