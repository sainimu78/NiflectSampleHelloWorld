#pragma once
#include "Niflect/NiflectAccessor.h"

namespace Niflect
{
	template <typename TValue>
	class TBuiltinTypeAccessor : public CNiflectAccessor
	{
	public:
		virtual bool SaveInstanceImpl(const InstanceType* base, CRwNode* rw) const override
		{
			auto& instance = *static_cast<const TValue*>(base);
			ASSERT(!rw->IsValue());
			auto rwValue = rw->ToValue();
			SetRwValueAs<TValue>(rwValue, instance);
			return true;
		}
		virtual bool LoadInstanceImpl(InstanceType* base, const CRwNode* rw) const override
		{
			auto& instance = *static_cast<TValue*>(base);
			ASSERT(rw->IsValue());
			auto rwValue = rw->GetValue();
			instance = GetRwValueAs<TValue>(rwValue);
			return true;
		}
	};

	using CBoolAccessor = TBuiltinTypeAccessor<bool>;
	using CUint8Accessor = TBuiltinTypeAccessor<uint8>;
	using CInt32Accessor = TBuiltinTypeAccessor<int32>;
	using CFloatAccessor = TBuiltinTypeAccessor<float>;
	using CStringAccessor = TBuiltinTypeAccessor<Niflect::CString>;
}