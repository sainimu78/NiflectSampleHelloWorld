#pragma once
#include "Niflect/NiflectAccessor.h"

namespace Niflect
{
	template <typename TValue>
	class TValueTypeAccessor : public CNiflectAccessor
	{
	public:
		virtual bool SaveInstanceImpl(const InstanceType* base, CRwNode* rw) const override
		{
			auto& instance = *static_cast<const TValue*>(base);
			NIFLECT_ASSERT(!rw->IsValue());
			auto rwValue = rw->ToValue();
			SetRwValueAs<TValue>(rwValue, instance);
			return true;
		}
		virtual bool LoadInstanceImpl(InstanceType* base, const CRwNode* rw) const override
		{
			auto& instance = *static_cast<TValue*>(base);
			NIFLECT_ASSERT(rw->IsValue());
			auto rwValue = rw->GetValue();
			instance = GetRwValueAs<TValue>(rwValue);
			return true;
		}
	};

	using CBoolAccessor = TValueTypeAccessor<bool>;
	using CUint8Accessor = TValueTypeAccessor<NifUint8>;
	using CInt32Accessor = TValueTypeAccessor<NifInt32>;
	using CFloatAccessor = TValueTypeAccessor<float>;
}