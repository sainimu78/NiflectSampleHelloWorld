#pragma once
#include "Niflect/Base/Memory.h"
#include "Niflect/Memory/Generic/GenericSharedPtr.h"

namespace Niflect
{
	template <typename T>
	using TSharedPtr = TGenericSharedPtr<T, CMemory>;

	template <typename T>
	inline static TSharedPtr<T> MakeSharable(T* rawPtr)
	{
		return GenericMakeSharable<T, CMemory>(rawPtr);
	}
	template <typename T, typename ...TArgs>
	inline static TSharedPtr<T> MakeShared(TArgs&& ...args)
	{
		return GenericMakeShared<T, CMemory>(std::forward<TArgs>(args)...);
	}
}