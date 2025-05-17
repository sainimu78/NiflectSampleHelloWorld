#pragma once
#include "Niflect/NiflectType.h"

namespace Niflect
{
	class CNiflectModule2;

	//为Module中所有CNiflectType的容器, 不需要被继承
	class CNiflectTable
	{
	public:
		CNiflectTable(CNiflectModule2* module = NULL)
			: m_module(module)
		{

		}
		void Init(const CString& name)
		{
			m_name = name;
		}
#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
		//如作修改, 须相应修改 NiflectGen\CodeWriter\TypeReg\ModuleRegisteredTypeHeaderCodeWriter.cpp 中关联位置 ModuleRegisteredTypeHeader
		template <typename TType, typename TInfo>
		void RegisterTypeDetailed(const Niflect::CString& id, const BuildTypeMetaFunc& inBuildTypeMetaFunc, CStaticNiflectTypeAddr* staticTypePtrAddr, const CSharedNata& nata, const CConstructorInfo& ctorInfo, const HashInt& typeHash)
		{
			auto shared = Niflect::MakeShared<TInfo>();
			CNiflectType* type = shared.Get();
			auto idx = this->GetTypesCount();
#ifdef NIFLECTDEV_GENERATED_TYPE_ALIGNMENT
			auto alignment = alignof(TType);
#else
			NifUint32 alignment = 0;
#endif
			type->InitTypeMeta(this, idx, sizeof(TType), alignment, &GenericInstanceInvokeDestructor<TType>, typeHash, id, inBuildTypeMetaFunc, staticTypePtrAddr, nata);
			if (ctorInfo.m_Func != NULL)
				type->m_vecConstructorInfo.push_back(ctorInfo);
			this->InsertType(shared, idx);
		}
		//如作修改, 须相应修改 NiflectGen\CodeWriter\TypeReg\ModuleRegisteredTypeHeaderCodeWriter.cpp 中关联位置 ModuleRegisteredTypeHeader
		template <typename TType, typename TInfo>
		void RegisterType(const Niflect::CString& id, const BuildTypeMetaFunc& inBuildTypeMetaFunc, const CSharedNata& nata, const InvokeMethodFunc& inInvokeConstructorFunc, const HashInt& typeHash)
		{
			NIFLECT_ASSERT(!TRegisteredType<TType>::IsValid());
			this->RegisterTypeDetailed<TType, TInfo>(id, inBuildTypeMetaFunc, &TRegisteredType<TType>::s_type, nata, CConstructorInfo(inInvokeConstructorFunc, NULL, typeHash, ""), typeHash);
			NIFLECT_ASSERT(TRegisteredType<TType>::IsValid());
		}
#else
		template <typename TInfo, typename TType>
		void RegisterType(const CString& typeName, const CreateFieldLayoutOfTypeFuncOld& Func)
		{
			NIFLECT_ASSERT(false);
			//STypeLifecycleFunctions typeFuncs;
			//typeFuncs.m_InvokeConstructorFunc = &GenericInstanceInvokeConstructor<TType>;
			//typeFuncs.m_InvokeDestructorFunc = &GenericInstanceInvokeDestructor<TType>;
			//typeFuncs.m_CreateTypeAccessorFunc = Func;

			//auto shared = MakeShared<TInfo>();
			//auto type = shared.Get();
			//auto idx = this->AddType(shared);
			//NIFLECT_ASSERT(!TRegisteredType<TType>::IsValid());
			//TRegisteredType<TType>::s_type = type;
			////type->InitStaticType<TType>();
			//type->InitTypeMeta(sizeof(TType), CNiflectType::GetTypeHash<TType>(), typeName, idx, typeFuncs);
			//NIFLECT_ASSERT(TRegisteredType<TType>::IsValid());
		}
		template <typename TType, typename TInfo = CNiflectType>
		void RegisterType2(const Niflect::CString& id, const CreateTypeAccessorFunc& Func)
		{
			NIFLECT_ASSERT(false);
			//CTypeInvokations typeFuncs;
			//typeFuncs.m_InvokeConstructorFunc = &GenericInstanceInvokeConstructor<TType>;
			//typeFuncs.m_InvokeDestructorFunc = &GenericInstanceInvokeDestructor<TType>;
			//typeFuncs.m_CreateTypeAccessorFunc = Func;

			//auto shared = Niflect::MakeShared<TInfo>();
			//CNiflectType* type = shared.Get();
			//auto idx = this->AddType(shared);
			//NIFLECT_ASSERT(!TRegisteredType<TType>::IsValid());
			//type->InitTypeMeta2(sizeof(TType), CNiflectType::GetTypeHash<TType>(), idx, typeFuncs, id, &TRegisteredType<TType>::s_type, NULL);
			//NIFLECT_ASSERT(TRegisteredType<TType>::IsValid());
		}
		template <typename TType, typename TInfo>
		void RegisterTypeDetailed(const Niflect::CString& id, const CreateTypeAccessorFunc& inCreateTypeAccessorFunc, CStaticNiflectTypeAddr* staticTypePtrAddr, const CSharedNata& nata, const InvokeConstructorFunc& InvokeConstructorFunc)
		{
			STypeLifecycleMeta lifecycleMeta;
			lifecycleMeta.m_typeSize = sizeof(TType);
			NIFLECT_ASSERT(InvokeConstructorFunc != NULL);
			lifecycleMeta.m_InvokeConstructorFunc = InvokeConstructorFunc;
			lifecycleMeta.m_InvokeDestructorFunc = &GenericInstanceInvokeDestructor<TType>;

			auto shared = Niflect::MakeShared<TInfo>();
			CNiflectType* type = shared.Get();
			auto idx = this->GetTypesCount();
			type->InitTypeMeta2(this, idx, lifecycleMeta, CNiflectType::GetTypeHash<TType>(), id, inCreateTypeAccessorFunc, staticTypePtrAddr, nata);
			this->InsertType(shared, idx);
		}
		template <typename TType, typename TInfo>
		void RegisterTypeChecked(const Niflect::CString& id, const CreateTypeAccessorFunc& inCreateTypeAccessorFunc, const CSharedNata& nata, const InvokeConstructorFunc& InvokeConstructorFunc)
		{
			NIFLECT_ASSERT(!TRegisteredType<TType>::IsValid());
			this->RegisterTypeDetailed<TType, TInfo>(id, inCreateTypeAccessorFunc, &TRegisteredType<TType>::s_type, nata, InvokeConstructorFunc);
			NIFLECT_ASSERT(TRegisteredType<TType>::IsValid());
		}
		template <typename TType, typename TInfo>
		void RegisterType3(const Niflect::CString& id, const CreateTypeAccessorFunc& inCreateTypeAccessorFunc, const CSharedNata& nata)
		{
			this->RegisterTypeChecked<TType, TInfo>(id, inCreateTypeAccessorFunc, nata, &GenericInstanceInvokeConstructor<TType>);
		}
#endif
		CNiflectModule2* GetModule() const
		{
			return m_module;
		}
		NifUint32 GetTypesCount() const
		{
			return static_cast<NifUint32>(m_vecType.size());
		}
		CNiflectType* GetTypeByIndex(NifUint32 idx) const
		{
			return m_vecType[idx].Get();
		}
		CNiflectType* FindTypeByTypeName(const Niflect::CString& id) const
		{
			auto itFound = m_mapIdToIndex.find(id);
			if (itFound != m_mapIdToIndex.end())
				return m_vecType[itFound->second].Get();
			return NULL;
		}
		NIFLECT_API void BuildTypesMeta() const;
		void InsertType(const CSharedNiflectType& type, NifUint32 idx)
		{
			auto ret = m_mapIdToIndex.insert({ type->GetTypeName(), idx });
			NIFLECT_ASSERT(ret.second);
			m_vecType.insert(m_vecType.begin() + idx, type);
		}

	public:
		void DeleteType(const CNiflectType* type)//备用
		{
			auto itFound = m_mapIdToIndex.find(type->GetTypeName());
			NIFLECT_ASSERT(itFound != m_mapIdToIndex.end());
			m_vecType.erase(m_vecType.begin() + itFound->second);
			m_mapIdToIndex.erase(itFound);
		}

	private:
		TArrayNif<CSharedNiflectType> m_vecType;
		TUnorderedMap<Niflect::CString, NifUint32> m_mapIdToIndex;
		CString m_name;
		CNiflectModule2* m_module;
	};
	using CSharedTable = TSharedPtr<CNiflectTable>;

#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
#else
	template <typename TInfo, typename TType>
	class TStaticTableTypeReg
	{
	public:
		TStaticTableTypeReg(CNiflectTable* table, const CString& typeName, const CreateFieldLayoutOfTypeFuncOld& Func)
		{
			table->RegisterType<TInfo, TType>(typeName, Func);
		}
		template <typename TNatimeta>
		TStaticTableTypeReg(CNiflectTable* table, const CString& typeName, const CreateFieldLayoutOfTypeFuncOld& Func, const TNatimeta& natimeta)
			: TStaticTableTypeReg(table, typeName, Func)
		{
			auto type = StaticGetType<TType>();
			type->SetNatimeta(MakeShared<TNatimeta>(natimeta));
		}
	};

	template <typename TType, typename TInfo>
	class TStaticTypeRegger
	{
	public:
		//TStaticTypeRegger(CNiflectTable* table, const Niflect::CString& id, const InvokeStaticCreateFieldLayoutFunc& Func)
		//{
		//	table->RegisterType2<TType, TInfo>(id, Func);
		//}
		template <typename TNata>
		TStaticTypeRegger(CNiflectTable* table, const Niflect::CString& id, const CreateTypeAccessorFunc& Func, const TNata& nata)
			//: TStaticTypeRegger(table, id, Func)
		{
			table->RegisterType2<TType, TInfo>(id, Func);
			if (CNiflectType::GetTypeHash<TNata>() != CNiflectType::GetTypeHash<CNata>())
			{
				auto type = StaticGetType<TType>();
				type->InitNata(Niflect::MakeShared<TNata>(nata));
			}
		}
	};

	template <typename TAccessor>
	static CSharedAccessor __InternalCreateFieldLayoutForFunctionPointer()
	{
		return MakeShared<TAccessor>();
	}

#define NIFLECT_MEMBER_FUNCTION_GET_TYPE_VIRTUAL()\
	public:\
		virtual Niflect::CNiflectType* GetType() const { return NULL; }

#define NIFLECT_MEMBER_FUNCTION_GET_TYPE_OVERRIDE(typeName)\
	public:\
		virtual Niflect::CNiflectType* GetType() const override\
		{\
			return Niflect::StaticGetType<typeName>();\
		}
	
//begin, 仅留作备用, 不需要考虑静态变量名冲突, 因为本就不能在同namespace中出现同名类型
//#define NIFLECT_STATIC_TYPE_REG_VAR_NAME_COMBINE_INNER(A, B) s_typeReg_##A##_##B
//#define NIFLECT_STATIC_TYPE_REG_VAR_NAME_COMBINE(A, B) NIFLECT_STATIC_TYPE_REG_VAR_NAME_COMBINE_INNER(A, B)
//#define NIFLECT_STATIC_TYPE_REG_VAR_NAME(typeName) NIFLECT_STATIC_TYPE_REG_VAR_NAME_COMBINE(typeName, __LINE__)
//end

#define DEPRECATED_NIFLECT_REGISTER(niflectType, typeName, invokeGetTable, customDataObject)\
	static Niflect::TStaticTableTypeReg<niflectType, typeName> s_typeReg_##typeName(invokeGetTable, #typeName, customDataObject);

#define NIFLECT_TYPE_REGISTER(typeName, niflectType, invokeGetTable, staticCreateFieldLayoutFuncAddr, nataType, nataObj)\
	static Niflect::TStaticTypeRegger<typeName, niflectType> s_typeReg_##typeName(invokeGetTable, #typeName, staticCreateFieldLayoutFuncAddr, nataType(nataObj))

#define NIFLECT_CLASS_REGISTER(typeName, invokeGetTable, staticCreateFieldLayoutFuncAddr, nataType, nataObj)\
	NIFLECT_TYPE_REGISTER(typeName, Niflect::CClass, invokeGetTable, staticCreateFieldLayoutFuncAddr, nataType, nataObj)
#endif
}