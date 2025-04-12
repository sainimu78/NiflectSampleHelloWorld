#pragma once
#include "Niflect/NiflectCommon.h"
#include "Niflect/Memory/AllocatorInterface.h"
#include "Niflect/Memory/Generic/GenericHeapAllocator.h"
#include "Niflect/Memory/Generic/GenericInstance.h"

namespace Niflect
{
	class CMemory
	{
	public:
		NIFLECT_API static void* Alloc(size_t size);
		NIFLECT_API static void* Realloc(void* ptr, size_t size);
		NIFLECT_API static void Free(void* ptr);

	public:
		NIFLECT_API static void SetCurrentAllocator(IAllocatorInterface* p);
	};

	class CInstance
	{
	public:
		template <typename TClass, typename ...TArgs>
		inline static TClass* New(TArgs&& ...args)
		{
			return CGenericInstance::New<CMemory, TClass>(args...);
		}
		template <typename TClass>
		inline static void Delete(TClass* obj)
		{
			CGenericInstance::Delete<CMemory, TClass>(obj);
		}
	};

	template <typename T>
	using THeapAllocator = TGenericHeapAllocator<T, CMemory>;
}