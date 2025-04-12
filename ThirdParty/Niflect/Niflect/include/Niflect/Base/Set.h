#pragma once
#include "Niflect/Base/Memory.h"
#include <set>

namespace Niflect
{
	template <typename TElement, typename TCompare, typename TAllocator>
	using TSTLSet = std::set<TElement, TCompare, TAllocator>;

	template <typename TElement, typename TCompare = std::less<TElement> >
	using TSet = TSTLSet<TElement, TCompare, THeapAllocator<TElement> >;
}