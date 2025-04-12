#pragma once
#include "Niflect/NiflectTable.h"

namespace Niflect
{
	typedef void (*ModuleRegisterTypesFunc)(CNiflectTable* table);
	typedef void (*ModuleInitTypesFunc)();

	class CNiflectModuleInfo
	{
	public:
		CNiflectModuleInfo()
			: m_RegisterTypesFunc(NULL)
			, m_InitTypesFunc(NULL)
		{
		}
		void InitMeta(const Niflect::CString& name, const ModuleRegisterTypesFunc& RegisterTypesFunc, const ModuleInitTypesFunc& InitTypesFunc)
		{
			m_name = name;
			m_RegisterTypesFunc = RegisterTypesFunc;
			m_InitTypesFunc = InitTypesFunc;
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
	};
	using CSharedModuleInfo = Niflect::TSharedPtr<CNiflectModuleInfo>;

	typedef Niflect::CNiflectModuleInfo* (*GeneratedGetModuleInfoFunc)();

	static Niflect::CString GetGeneratedGetModuleInfoFuncName(const Niflect::CString& moduleName)
	{
		constexpr const char* prefix = "NiflectGeneratedGetModuleInfo_";
		return prefix + moduleName;
	}
}