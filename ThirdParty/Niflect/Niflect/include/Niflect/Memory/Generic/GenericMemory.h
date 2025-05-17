#pragma once
#include "Niflect/Memory/Stats/NiflectMemoryStats.h"
#include <stdlib.h>//malloc, realloc, free

namespace Niflect
{	
	class CGenericMemory
	{
		//计划:
		//1.CDefaultMemory内存管理全局实例, CAlignedMemory可复用
		//2.分类内存池, 池使用情况, 垃圾回收(引用关系系统)
		//3.绑定反射信息, 同类内存堆叠摆放(ECS), 提高cache命中, 可能需要专门一种反射方式标明用于同类内存堆叠
		//4.线程堆管理, 线程ID与page映射, 需要确认不同平台的线程ID分配的值是否和常见平台如Windows相同小于65536
		//5.紧凑内存分布容器可行性构思和验证, 如Vector扩容后预留空间可被利用
		//6.内存优化方法调研
		
	private:
		typedef NifUint64 BytesType;

	private:
		struct SMemoryInfo
		{
			BytesType m_size;
		};

		//begin, 可复现CMemoryTest非最后释放问题的代码, 需要在最先调用CDefaultMemory::Alloc的函数内定义static CMyTest g;
		//找最先调用CDefaultMemory::Alloc的方法是下断点, 进断点后即可找到最先调用的函数
		//class CDDDDDDDDDDD
		//{
		//};
		//class CASSSSSS
		//{
		//public:
		//	CASSSSSS()
		//	{
		//		//m_a = ClassMemory::New<CDDDDDDDDDDD, CDefaultMemory>();
		//	}
		//	~CASSSSSS()
		//	{
		//		printf("");
		//	}
		//	ClassMemory::TLiteSharedPointer<CDDDDDDDDDDD, CDefaultMemory> m_a;
		//};
		//class CMyTest
		//{
		//public:
		//	CMyTest()
		//	{
		//		static TSingletonPtr<CASSSSSS> g_holder(new CASSSSSS);
		//		static auto g_singleton = g_holder.Ref();
		//		g_singleton->m_a = ClassMemory::New<CDDDDDDDDDDD, CDefaultMemory>();
		//	}
		//	~CMyTest()
		//	{
		//		printf("");
		//	}
		//};
		//end

	private:
		inline static void* InternalMalloc(BytesType sz)
		{
			return malloc(sz);
		}
		inline static void* InternalRealloc(void* p, BytesType sz)
		{
			return realloc(p, sz);
		}
		inline static void InternalFree(void* p)
		{
			free(p);
		}

	public:
		static void* Alloc(BytesType size, CMemoryStats* stats)
		{
			const auto memInfoSize = sizeof(SMemoryInfo);
			auto actualSize = memInfoSize + size;
			auto base = InternalMalloc(actualSize);
			auto memInfo = static_cast<SMemoryInfo*>(base);
			memInfo->m_size = size;
			if (stats != NULL)
			{
				stats->m_bytesRuntime += memInfo->m_size;
				stats->m_bytesMemoryInfo += memInfoSize;
				stats->m_allocCount++;
				stats->m_allocatedBytesRuntimeTotal += memInfo->m_size;
				stats->m_allocatedBytesTotal += memInfoSize + memInfo->m_size;
			}
			auto mem = static_cast<char*>(base) + memInfoSize;
			return mem;
		}
		static void* Realloc(void* mem, BytesType size, CMemoryStats* stats)
		{
			if (mem == NULL)
				return Alloc(size, stats);
			NIFLECT_ASSERT(size != 0);
			const auto memInfoSize = sizeof(SMemoryInfo);
			auto actualSize = memInfoSize + size;
			auto base = static_cast<char*>(mem) - memInfoSize;
			auto oldSize = reinterpret_cast<SMemoryInfo*>(base)->m_size;//todo: realloc应该能确保数据不变, 因此应该不必备份, 确认如此则移除备份
			auto newBase = InternalRealloc(base, actualSize);
			auto memInfo = static_cast<SMemoryInfo*>(newBase);
			memInfo->m_size = size;
			if (stats != NULL)
			{
				auto diff = oldSize - memInfo->m_size;
				if (oldSize > memInfo->m_size)
				{
					stats->m_freedBytesRuntimeTotal += diff;
					stats->m_freedBytesTotal += diff;
				}
				else
				{
					stats->m_allocatedBytesRuntimeTotal -= diff;
					stats->m_allocatedBytesTotal -= diff;
				}
				stats->m_bytesRuntime -= diff;
			}
			mem = static_cast<char*>(newBase) + memInfoSize;
			return mem;
		}
		static void Free(void* mem, CMemoryStats* stats)
		{
			if (mem == NULL)
				return;
			const auto memInfoSize = sizeof(SMemoryInfo);
			auto base = static_cast<char*>(mem) - memInfoSize;
			auto memInfo = reinterpret_cast<SMemoryInfo*>(base);
			if (stats != NULL)
			{
				NIFLECT_ASSERT(stats->m_bytesRuntime == memInfo->m_size || stats->m_bytesRuntime > memInfo->m_size);
				stats->m_bytesRuntime -= memInfo->m_size;
				NIFLECT_ASSERT(stats->m_bytesMemoryInfo == memInfoSize || stats->m_bytesMemoryInfo > memInfoSize);
				stats->m_bytesMemoryInfo -= memInfoSize;
				stats->m_freeCount++;
				stats->m_freedBytesRuntimeTotal += memInfo->m_size;
				stats->m_freedBytesTotal += memInfoSize + memInfo->m_size;
			}
			InternalFree(base);
		}
	};
}