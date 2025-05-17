#pragma once
#include "Niflect/Memory/AllocatorInterface.h"
#include "Niflect/NiflectBase.h"
#include "Niflect/Memory/Generic/GenericHeapAllocator.h"

namespace Niflect
{
	class CDefaultMemoryPoolScope : public IAllocatorInterface
	{
		struct SChunk
		{
			void* m_mem;
			size_t m_sizeUsaed;
		};
	public:
		CDefaultMemoryPoolScope()
			: m_chunkSize(32)//测试用
		{
			//需要考虑嵌套的情况?
			CMemory::SetCurrentAllocator(this);
		}
		~CDefaultMemoryPoolScope()
		{
			CMemory::SetCurrentAllocator(NULL);
			for (auto& it : m_vecChunk)
				CDefaultMemory::Free(it.m_mem);
		}
		virtual void* Alloc(size_t size) override
		{
			size_t sizeUsed = 0;
			if (m_vecChunk.size() > 0)
				sizeUsed = m_vecChunk.back().m_sizeUsaed;

			{
				auto maxChunkSize = sizeUsed > m_chunkSize ? sizeUsed : m_chunkSize;
				if (maxChunkSize - sizeUsed < size || m_vecChunk.size() == 0)
					m_vecChunk.push_back({ CDefaultMemory::Alloc(size > m_chunkSize ? size : m_chunkSize), 0 });
			}

			auto& chunk = m_vecChunk.back();
			auto addr = static_cast<char*>(chunk.m_mem) + chunk.m_sizeUsaed;
			chunk.m_sizeUsaed += size;
			return addr;
		}
		virtual void* Realloc(void* ptr, size_t size) override
		{
			return this->Alloc(size);
		}
		virtual void Free(void* ptr) override
		{
		}

	private:
		//略显特殊, 因为改变了全局 CMemory 的 allocator, 因此如果动态成员仍用 CMemory 的分配函数, 将导致嵌套执行本类的 Alloc
		//因此使用 DefaultMemory, 当然使用 CRT 或 std::vector 的默认 Allocator 都是可行的, 只要不用 CMemory 的分配即可
		std::vector<SChunk, TGenericHeapAllocator<SChunk, CDefaultMemory> > m_vecChunk;
		const NifUint32 m_chunkSize;
	};
}