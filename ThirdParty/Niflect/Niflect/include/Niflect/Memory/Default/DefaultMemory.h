#pragma once
#include "Niflect/NiflectCommon.h"
#include "Niflect/Memory/Stats/NiflectMemoryStats.h"

namespace Niflect
{
	class CDefaultMemory
	{
	public:
		NIFLECT_API static void* Alloc(size_t size);
		NIFLECT_API static void* Realloc(void* ptr, size_t size);
		NIFLECT_API static void Free(void* ptr);

	public:
		NIFLECT_API static void PushStats(CMemoryStats* stats, CMemoryStats*& lastStats);
		NIFLECT_API static void PopStats(CMemoryStats* lastStats);
	};

	using CDefaultMemoryStatsScope = TStackedStatsScope<CDefaultMemory>;

	NIFLECT_API CMemoryStats* GetDefaultMemoryStats();

	NIFLECT_API void TestMemoryStatsOnThreadsBegin();
	NIFLECT_API void TestMemoryStatsOnThreadsEnd();
}