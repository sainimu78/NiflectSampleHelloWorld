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
#include "Niflect/NiflectCommon.h"

namespace Niflect
{
	class CNiflectType;

	typedef void (*InvokeMethodFunc)(InstanceType* base, InstanceType** const args);
	typedef void (*InvokeFunctionFunc)(InstanceType** const args);

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
			, m_signatureHash(INVALID_HASH)
		{
		}
		CConstructorInfo(const InvokeMethodFunc& Func, const CSharedNata& nata, const HashInt& signatureHash, const Niflect::CString& argsSignature)
			: m_Func(Func)
			, m_nata(nata)
			, m_signatureHash(signatureHash)
			, m_argsSignature(argsSignature)
		{
		}
		Niflect::TArray<CParameterInfo> m_vecInput;
		InvokeMethodFunc m_Func;
		CSharedNata m_nata;
		HashInt m_signatureHash;
		Niflect::CString m_argsSignature;
	};

	class CMethodInfo
	{
	public:
		CMethodInfo()
			: m_Func(NULL)
			, m_signatureHash(INVALID_HASH)
		{
		}
		CMethodInfo(const InvokeMethodFunc& Func, const Niflect::CString& name, const CSharedNata& nata, const HashInt& signatureHash, const Niflect::CString& argsSignature)
			: m_Func(Func)
			, m_name(name)
			, m_signatureHash(signatureHash)
			, m_argsSignature(argsSignature)
			, m_nata(nata)
		{
		}
		Niflect::TArray<CParameterInfo> m_vecInput;
		Niflect::TArray<CParameterInfo> m_vecOutput;
		InvokeMethodFunc m_Func;
		HashInt m_signatureHash;
		Niflect::CString m_name;
		Niflect::CString m_argsSignature;
		CSharedNata m_nata;
	};

	class CFunctionInfo
	{
	public:
		CFunctionInfo()
			: m_Func(NULL)
			, m_signatureHash(INVALID_HASH)
		{
		}
		CFunctionInfo(const InvokeFunctionFunc& Func, const Niflect::CString& name, const CSharedNata& nata, const HashInt& signatureHash, const Niflect::CString& argsSignature)
			: m_Func(Func)
			, m_name(name)
			, m_signatureHash(signatureHash)
			, m_argsSignature(argsSignature)
			, m_nata(nata)
		{
		}
		Niflect::TArray<CParameterInfo> m_vecInput;
		Niflect::TArray<CParameterInfo> m_vecOutput;
		InvokeFunctionFunc m_Func;
		HashInt m_signatureHash;
		Niflect::CString m_name;
		Niflect::CString m_argsSignature;
		CSharedNata m_nata;
	};

	NIFLECT_API HashInt ComputeSignatureHash(const Niflect::CString& str);
}