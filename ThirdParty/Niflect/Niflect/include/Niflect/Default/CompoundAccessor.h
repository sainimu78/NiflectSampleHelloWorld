#pragma once
#include "Niflect/NiflectAccessor.h"

namespace Niflect
{
	class CCompoundAccessor : public CNiflectAccessor
	{
	public:
		virtual bool SaveInstanceImpl(const InstanceType* base, CRwNode* rw) const override
		{
			for (auto& it : this->GetOwnerType()->GetFields())
			{
				ASSERT(!it.GetName().empty());
				auto rwField = CreateRwNode();
				if (it.TypeSaveInstanceToRwNode(base, rwField.Get()))
					AddExistingRwNode(rw, it.GetName(), rwField);
			}
			return true;
		}
		virtual bool LoadInstanceImpl(InstanceType* base, const CRwNode* rw) const override
		{
			for (auto& it : this->GetOwnerType()->GetFields())
			{
				ASSERT(!it.GetName().empty());
				auto rwChild = FindRwNode(rw, it.GetName());
				it.TypeLoadInstanceFromRwNode(base, rwChild);
			}
			return true;
		}
		virtual bool BuildInstanceNodeImpl(CNiflectInstanceNode* node) const override
		{
			for (auto& it : this->GetOwnerType()->GetFields())
			{
				ASSERT(!it.GetName().empty());
				auto fieldNode = CreateInstanceNode();
				if (it.TypeBuildInstanceNode(node, fieldNode.Get()))
					node->AddNode(fieldNode);
			}
			return true;
		}
	};
}