#pragma once
#include "Niflect/NiflectAccessor.h"

namespace Niflect
{
	class CCompoundAccessor : public CAccessor
	{
	public:
		virtual bool SaveInstanceImpl(const InstanceType* base, CRwNode* rw) const override
		{
			for (auto& it : this->GetFields())
			{
				ASSERT(!it.GetName().empty());
				auto rwField = CreateRwNode();
				if (it.LayoutSaveToRwNode(base, rwField.Get()))
					AddExistingRwNode(rw, it.GetName(), rwField);
			}
			return true;
		}
		virtual bool LoadInstanceImpl(InstanceType* base, const CRwNode* rw) const override
		{
			for (auto& it : this->GetFields())
			{
				ASSERT(!it.GetName().empty());
				auto rwChild = FindRwNode(rw, it.GetName());
				it.LayoutLoadFromRwNode(base, rwChild);
			}
			return true;
		}
	};
}