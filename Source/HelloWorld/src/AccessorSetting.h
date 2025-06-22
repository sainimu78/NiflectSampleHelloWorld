#pragma once
#include "Niflect/Component/RwTree/Accessor/StandardRwAccessorSetting.h"
#include "Resource.h"

using namespace RwTree;

class CResourceAccessor : public CRwAccessor
{
    using PointerType = CResource*;
protected:
    virtual bool SaveImpl(const InstanceType* base, CRwNode* rw) const
    {
        auto& instance = *static_cast<const PointerType*>(base);
        SetRwValueAs<Niflect::CString>(rw->ToValue(), *instance->m_id);
        return true;
    }
    virtual bool LoadImpl(InstanceType* base, const CRwNode* rw) const
    {
        auto& instance = *static_cast<PointerType*>(base);
        instance = GetResourceFactory()->Find(GetRwValueAs<Niflect::CString>(rw->GetValue()));
        return true;
    }
};

using namespace NiflectAccessorSetting;

NIFAS_A() TSetting<CResourceAccessor, CResource*>;