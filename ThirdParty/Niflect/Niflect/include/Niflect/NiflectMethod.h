//#pragma once
//#include "Niflect/Accessor/NiflectAccessor.h"
//
//namespace Niflect
//{
//	typedef void (*InvokeMethodFunc)(void** paramAddrArray, const TArrayNif<CSharedAccessor>& vecInput, const TArrayNif<CSharedAccessor>& vecOutput);
//
//	class CNiflectMethod
//	{
//	public:
//		CNiflectMethod()
//			: m_InvokeFunc(NULL)
//		{
//		}
//		void Invoke(void** paramAddrArray) const
//		{
//			m_InvokeFunc(paramAddrArray, m_vecInputAccessor, m_vecOutputAccessor);
//		}
//		TArrayNif<CSharedAccessor> m_vecOutputAccessor;
//		TArrayNif<CSharedAccessor> m_vecInputAccessor;
//		InvokeMethodFunc m_InvokeFunc;
//	};
//}

#pragma once
#include "Niflect/Base/Array.h"
#include "Niflect/NiflectAddr.h"
#include "Niflect/Base/String.h" 
#include "Niflect/NiflectNata.h"

namespace Niflect
{
	class CNiflectType;

	typedef void (*InvokeMethodFunc)(InstanceType* base, InstanceType** const args);

	template <typename TType>
	static void InvokeDefaultConstructor(InstanceType* base, InstanceType** const args)
	{
		GenericInstanceInvokeConstructor<TType>(base);
	}

	class CParameterInfo
	{
	public:
		CParameterInfo()
			: m_type(NULL)
			, m_isConstant(false)
			, m_isReference(false)
		{
		}
		CParameterInfo(CNiflectType* type, bool isConstant, bool isReference)
			: m_type(type)
			, m_isConstant(isConstant)
			, m_isReference(isReference)
		{

		}
		CNiflectType* m_type;
		bool m_isConstant;
		bool m_isReference;
	};

	class CConstructorInfo
	{
	public:
		CConstructorInfo()
			: m_Func(NULL)
		{
		}
		CConstructorInfo(const InvokeMethodFunc& Func, const CSharedNata& nata)
			: m_Func(Func)
			, m_nata(nata)
		{
		}
		Niflect::TArray<CParameterInfo> m_vecInput;
		InvokeMethodFunc m_Func;
		CSharedNata m_nata;
	};

	class CMethodInfo
	{
	public:
		CMethodInfo()
			: m_Func(NULL)
		{
		}
		CMethodInfo(const InvokeMethodFunc& Func, const Niflect::CString& name, const CSharedNata& nata)
			: m_Func(Func)
			, m_name(name)
			, m_nata(nata)
		{
		}
		//inline void Invoke(InstanceType* base, InstanceType** inputArray, InstanceType** ouputArray) const
		//{
		//	m_InvokeMethodFunc(base, inputArray, ouputArray);
		//}
		Niflect::TArray<CParameterInfo> m_vecInput;
		Niflect::TArray<CParameterInfo> m_vecOutput;
		InvokeMethodFunc m_Func;
		Niflect::CString m_name;
		CSharedNata m_nata;
	};
}