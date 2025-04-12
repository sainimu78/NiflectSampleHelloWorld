#pragma once
#include "Niflect/NiflectCommon.h"

namespace Niflect
{
	//该内存统计主要用于:
	//1. 方便开发阶段确保内存处理正确性
	//2. 查看所实现方法的内存消耗
	//3. 复杂对象内存snapshot
	//不用profiler等工具的原因:
	//1. 可在调试过程中直接看统计结果
	//2. 方便修改需要统计信息
	//3. 方便对一些实现进行针对性测试
	class CMemoryStats
	{
	public:
		CMemoryStats()
			: m_bytesRuntime(0)
			, m_bytesMemoryInfo(0)
			, m_allocCount(0)
			, m_freeCount(0)
			, m_allocatedBytesRuntimeTotal(0)
			, m_allocatedBytesTotal(0)
			, m_freedBytesRuntimeTotal(0)
			, m_freedBytesTotal(0)
		{

		}
		~CMemoryStats()
		{
		}
		void DebugCheck()
		{
			//早于2023.08.21的结论
			//遗留缺陷, 无法保证该测试类最后释放
			//计划实现全局单例专门的管理对象, 由该管理对象分配释放这些单例, 以保证释放顺序
			//2023.08.21, 只需要保证该测试对象是最先执行完构造函数, 就能保证最后析构
			//复现非最后释放问题的代码见下注释区域
			//因此结论是, 为使CMemoryTest最先执行完构造函数, 其它静态变量创建也需要用CDefaultMemory, 不用new
			//如果用new, 则需要保证不在new执行完后调用CDefaultMemory::Alloc
			//更好的办法是统一使用CDefaultMemory后, TSingletonPtr也支持CDefaultMemory, CMemoryTest不纳入管控则用new即可
			ASSERT(m_bytesRuntime == 0);
			ASSERT(m_bytesMemoryInfo == 0);
			ASSERT(m_allocCount == m_freeCount);
			ASSERT(m_allocatedBytesRuntimeTotal == m_freedBytesRuntimeTotal);
			ASSERT(m_allocatedBytesTotal == m_freedBytesTotal);
			ASSERT(m_allocatedBytesTotal == 0 || m_allocatedBytesTotal > m_allocatedBytesRuntimeTotal);
			ASSERT(m_freedBytesTotal == 0 || m_freedBytesTotal > m_freedBytesRuntimeTotal);
		}
		size_t m_bytesRuntime;
		size_t m_bytesMemoryInfo;
		size_t m_allocCount;
		size_t m_freeCount;
		size_t m_allocatedBytesRuntimeTotal;
		size_t m_allocatedBytesTotal;
		size_t m_freedBytesRuntimeTotal;
		size_t m_freedBytesTotal;
	};

	template <typename TMemory>
	class TStackedStatsScope
	{
	public:
		TStackedStatsScope()
			: m_lastStats(NULL)
		{
			TMemory::PushStats(&m_stats, m_lastStats);
		}
		~TStackedStatsScope()
		{
			TMemory::PopStats(m_lastStats);
			m_stats.DebugCheck();
		}

	public:
		class CDisabled
		{
		public:
			CDisabled(TStackedStatsScope& parentScope)
				: m_parentScope(parentScope)
			{
				CMemoryStats* placeholder;
				TMemory::PushStats(m_parentScope.m_lastStats, placeholder);
				ASSERT(placeholder == &m_parentScope.m_stats);
			}
			~CDisabled()
			{
				TMemory::PopStats(&m_parentScope.m_stats);
			}

		private:
			TStackedStatsScope& m_parentScope;
		};

	private:
		CMemoryStats m_stats;
		CMemoryStats* m_lastStats;
	};
}