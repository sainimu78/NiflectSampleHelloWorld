#pragma once
#include "Niflect/NiflectAccessor.h"
#include "Niflect/NiflectType.h"

namespace Niflect
{
	class CEnumClassAccessor : public CNiflectAccessor
	{
		typedef CNiflectAccessor inherited;
	public:
		virtual bool SaveInstanceImpl(const InstanceType* base, CRwNode* rw) const override
		{
			//typename std::underlying_type<TEnum>::type >, 可获取枚举对应的整数类型

			auto et = this->GetEnum();
			auto rwValue = rw->ToValue();
			NifUint32 idx = NifInvalidIndex;
			switch (et->GetTypeSize())
			{
			case 1: idx = *static_cast<const NifUint8*>(base); break;
			case 2: idx = *static_cast<const NifUint16*>(base); break;
			case 4: idx = *static_cast<const NifUint32*>(base); break;
			default:
				NIFLECT_ASSERT(false);
				break;
			}
			auto& name = et->GetEnumConstNameByIndex(idx);
			rwValue->SetString(name);
			return true;
		}
		virtual bool LoadInstanceImpl(InstanceType* base, const CRwNode* rw) const override
		{
			auto et = this->GetEnum();
			auto rwValue = rw->GetValue();
			auto name = rwValue->GetString();
			auto idx = et->FindEnumConstMetaIndex(name);
			NIFLECT_ASSERT(idx != NifInvalidIndex);//可能由于此枚举类型的版本不同导致查找失败
			switch (et->GetTypeSize())
			{
			case 1: *static_cast<NifUint8*>(base) = idx; break;
			case 2: *static_cast<NifUint16*>(base) = idx; break;
			case 4: *static_cast<NifUint32*>(base) = idx; break;
			default:
				NIFLECT_ASSERT(false);
				break;
			}
			return true;
		}

	private:
		CEnum* GetEnum() const
		{
			return CEnum::Cast(inherited::GetOwnerType());
		}
	};

	class CEnumBitMaskAccessor : public CNiflectAccessor
	{
		typedef CNiflectAccessor inherited;
	public:
		virtual bool SaveInstanceImpl(const InstanceType* base, CRwNode* rw) const override
		{
			//typename std::underlying_type<TEnum>::type >, 可获取枚举对应的整数类型
			return true;
		}
		virtual bool LoadInstanceImpl(InstanceType* base, const CRwNode* rw) const override
		{
			return true;
		}

	private:
		CEnum* GetEnum() const
		{
			return CEnum::Cast(inherited::GetOwnerType());
		}
	};
}