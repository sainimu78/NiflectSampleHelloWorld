#pragma once
#include "Niflect/NiflectModuleInfo.h"

namespace Niflect
{
	class CNiflectModule
	{
	public:
		CNiflectModule()
			: m_nativeHandle(NULL)
			, m_info(NULL)
		{
		}
		void Init(void* nativeHandle, CNiflectModuleInfo* info)
		{
			m_nativeHandle = nativeHandle;
			m_info = info;
		}

	public:
		CNiflectModuleInfo* m_info;

	private:
		void* m_nativeHandle;
	};

	class CNiflectModuleManager
	{
	public:
		NIFLECT_API bool RegisterStaticlyLoadedModule(const Niflect::CString& moduleName);
		uint32 GetModulesCount() const
		{
			return static_cast<uint32>(m_vecModule.size());
		}
		const CNiflectModule& GetModule(uint32 idx) const
		{
			return m_vecModule[idx];
		}

	private:
		Niflect::TArrayNif<CNiflectModule> m_vecModule;
		Niflect::TMap<Niflect::CString, uint32> m_mapNameToIdx;
	};
}