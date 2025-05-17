#pragma once
#include "Niflect/NiflectTable.h"
//#include "Niflect/Field/ArrayField.h"
//#include "Niflect/Field/MapField.h"

#ifdef WIN32
#define NIFLECTMODULEREG_API extern "C" __declspec(dllexport)
#else
#define NIFLECTMODULEREG_API extern "C" __attribute__((visibility("default")))
#endif

namespace Niflect
{
	class CNiflectRegistration;
	using CSharedRegistration = TSharedPtr<CNiflectRegistration>;
}

typedef Niflect::CNiflectRegistration* (*FuncGeneratedNiflectRegistrationGet)();
typedef void (*FuncGeneratedNiflectRegistrationRelease)();

//NIFLECTMODULEREG_API Niflect::CNiflectRegistration* GeneratedNiflectRegistrationGet();
static Niflect::CNiflectRegistration* GeneratedNiflectRegistrationGet()
{
	return NULL;
}
NIFLECTMODULEREG_API void GeneratedNiflectRegistrationRelease();
NIFLECTMODULEREG_API void GeneratedNiflectRegistrationInitialReg();
NIFLECTMODULEREG_API void GeneratedNiflectRegistrationInitTypes();

namespace Niflect
{
	class CNiflectRegistration
	{
	public:
		CNiflectRegistration()
			: m_miscTableIndex(NifInvalidIndex)
		{
		}
		~CNiflectRegistration()
		{
		}

	public:
		void InitMiscTable()
		{
			NIFLECT_ASSERT(m_miscTableIndex == NifInvalidIndex);
			m_miscTableIndex = this->GetTablesCount();
			this->AddNewTable();
		}
	//public:
	//	void InitTables()
	//	{
	//		this->DoInitTables();
	//		this->AddMiscTable();
	//	}
	//	void RegisterTypes()
	//	{
	//		this->DoRegisterTypes();
	//	}
	//	void InitTypes() const
	//	{
	//		this->DoInitTypes();
	//	}
	//	void InitMethods() const
	//	{
	//		this->DoInitMethods();
	//	}
	//	void InitTypesAccessorTree()
	//	{
	//		//#3, 创建AccessorTree, 主要用于序列化
	//		for (NifUint32 idx0 = 0; idx0 < this->GetTablesCount(); ++idx0)
	//		{
	//			auto& table = this->GetMutableTable(idx0);
	//			for (auto& it : table.m_vecType)
	//				it->InitFieldLayout();
	//		}
	//	}

	//protected:
	//	virtual void DoInitTables() = 0;
	//	virtual void DoRegisterTypes() = 0;
	//	virtual void DoInitTypes() const = 0;
	//	virtual void DoInitMethods() const = 0;

	public:
		CNiflectTable* AddNewTable()
		{
			m_vecTable.push_back(CNiflectTable());
			return &m_vecTable.back();
		}
		void ClearTables()//todo: 实际上还需要通过引用关系安全删除
		{
			m_vecTable.clear();
		}
		NifUint32 GetTablesCount() const
		{
			return static_cast<NifUint32>(m_vecTable.size());
		}
		CNiflectTable* GetMutableTable(NifUint32 idx)
		{
			return &m_vecTable[idx];
		}
		CNiflectTable* GetMutableMiscTable()
		{
			return &m_vecTable[m_miscTableIndex];
		}

	public:
		static const CNiflectRegistration* Get()
		{
			return GeneratedNiflectRegistrationGet();
		}
		static CNiflectRegistration* GetMutable()
		{
			return GeneratedNiflectRegistrationGet();
		}
		static void Release()
		{
			GeneratedNiflectRegistrationRelease();
		}
		static void InitialReg()
		{
			GeneratedNiflectRegistrationInitialReg();
		}
		static void InitTypes()
		{
			GeneratedNiflectRegistrationInitTypes();
		}


		//template <typename TDerivedReg>
		//static void StaticCreate()
		//{
		//	s_reg = MakeShared<TDerivedReg>();
		//}
		//static void StaticDestroy()
		//{
		//	s_reg = NULL;
		//}

	private:
		TArrayNif<CNiflectTable> m_vecTable;
		NifUint32 m_miscTableIndex;

	//private:
	//	static InitialRegFunc m_InitialRegFunc;
	};

	//template <typename TField, typename TType>
	//static CSharedField __InternalUseForFunctionPointer()
	//{
	//	return CreateField<TField, TType>();
	//}

	//template <typename TField, typename TType>
	//static CNiflectType* StaticGetType_Misc(const CString& typeName)
	//{
	//	if (!TInternalRegisteredType<TType>::IsValid())
	//	{
	//		auto reg = CNiflectRegistration::Get();
	//		auto& table = reg->GetMutableMiscTable();
	//		NIFLECT_ASSERT(!typeName.empty());
	//		table.RegisterType<CNiflectType, TType>(typeName, &__InternalUseForFunctionPointer<TField, TType>, typeid(TType).hash_code());
	//	}
	//	return StaticGetType<TType>();
	//}
}