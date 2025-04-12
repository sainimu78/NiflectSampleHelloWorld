#pragma once
#include "Niflect/Base/Memory.h"
#include "Niflect/Util/sorted_vector.h"

namespace Niflect
{
	template <typename TKey, typename TCompare, typename TAllocator>
	using TSortedVector = codeproject::sorted_vector<TKey, true, TCompare, TAllocator>;

	template <typename TElement>
	using TSortedArray = TSortedVector<TElement, THeapAllocator<TElement> >;
}