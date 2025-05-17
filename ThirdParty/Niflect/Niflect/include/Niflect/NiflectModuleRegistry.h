#pragma once
#include "Niflect/NiflectTable.h"

namespace Niflect
{
	typedef void (*ModuleRegisterTypesFunc)(CNiflectTable* table);
	typedef void (*ModuleInitTypesFunc)();

	class CNiflectModule2
	{
		friend class CNiflectModuleRegistry;
	public:
		CNiflectModule2()
			: m_indexInManager(NifInvalidIndex)
			, m_RegisterTypesFunc(NULL)
			, m_InitTypesFunc(NULL)
			, m_table(this)
		{
		}
		void InitMeta(const Niflect::CString& name, NifUint32 moduleIdx, const ModuleRegisterTypesFunc& RegisterTypesFunc, const ModuleInitTypesFunc& InitTypesFunc)
		{
			m_name = name;
			m_RegisterTypesFunc = RegisterTypesFunc;
			m_InitTypesFunc = InitTypesFunc;
			m_indexInManager = moduleIdx;
		}
		void RegisterTypes()
		{
			m_RegisterTypesFunc(&m_table);
		}
		void InitTypes()
		{
			m_InitTypesFunc();
		}
		void BuildTableTypesMeta()
		{
			m_table.BuildTypesMeta();
		}
		const Niflect::CString& GetName() const
		{
			return m_name;
		}
		CNiflectTable* GetTable()
		{
			return &m_table;
		}

	private:
		Niflect::CString m_name;
		CNiflectTable m_table;
		ModuleRegisterTypesFunc m_RegisterTypesFunc;
		ModuleInitTypesFunc m_InitTypesFunc;
		NifUint32 m_indexInManager;
	};
	using CSharedModule2 = Niflect::TSharedPtr<CNiflectModule2>;

	class CNiflectModuleRegistry
	{
		friend class CNiflectModuleRegger;
	public:
		NIFLECT_API void InitRegisteredModules();
		NIFLECT_API NifUint32 GetModulesCount() const;
		NIFLECT_API CNiflectModule2* GetModuleByIndex(NifUint32 idx) const;

	private:
		NIFLECT_API bool RegisterModuleStatically(const Niflect::CString& name, const ModuleRegisterTypesFunc& RegisterTypesFunc, const ModuleInitTypesFunc& InitTypesFunc);
		NIFLECT_API static CNiflectModuleRegistry* InitializeSingleton();

	private:
		Niflect::TArrayNif<CSharedModule2> m_vecModule;
	};

	NIFLECT_API CNiflectModuleRegistry* GetModuleRegistry();
	NIFLECT_API void CleanupModuleRegistry_reserved();

	class CNiflectModuleRegger
	{
	public:
		CNiflectModuleRegger(const Niflect::CString& name, const ModuleRegisterTypesFunc& RegisterTypesFunc, const ModuleInitTypesFunc& InitTypesFunc)
		{
			auto mgr = GetModuleRegistry();
			if (mgr == NULL)
				mgr = CNiflectModuleRegistry::InitializeSingleton();
			mgr->RegisterModuleStatically(name, RegisterTypesFunc, InitTypesFunc);
		}
	};
}