#pragma once
#include "Niflect/Base/SharedPtr.h"
#include "Niflect/Base/Array.h"
#include "Niflect/Base/String.h"
#include "Niflect/Serialization/RwTree.h"
#include "Niflect/NiflectAddr.h"

namespace Niflect
{
	//using namespace RwTree;

	//class CNiflectSaveLoad
	//{
	//public:
	//	CNiflectSaveLoad()
	//		: m_base(NULL)
	//	{

	//	}
	//	void Init(InstanceType* base)
	//	{
	//		ASSERT(m_base == NULL);
	//		m_base = base;
	//	}

	//public:
	//	virtual bool Save(CRwNode* rw) const = 0;
	//	virtual bool Load(const CRwNode* rw) const = 0;

	//protected:
	//	InstanceType* m_base;
	//};
	//using CSharedSaveLoad = Niflect::TSharedPtr<CNiflectSaveLoad>;

	//class CNiflectInstanceNode;
	//using CSharedInstanceNode = Niflect::TSharedPtr<CNiflectInstanceNode>;

	//class CNiflectInstanceNode
	//{
	//public:
	//	CNiflectInstanceNode()
	//		: m_base(NULL)
	//	{

	//	}
	//	void InitAddr(InstanceType* base)
	//	{
	//		ASSERT(m_base == NULL);
	//		m_base = base;
	//		if (m_delegate != NULL)
	//			m_delegate->Init(m_base);
	//	}
	//	template <typename T>
	//	void InitDelegate()
	//	{
	//		ASSERT(m_delegate == NULL);
	//		m_delegate = Niflect::MakeShared<T>();
	//	}
	//	void AddNode(const CSharedInstanceNode& node)
	//	{
	//		return m_vecNode.push_back(node);
	//	}
	//	uint32 GetNodesCount() const
	//	{
	//		return static_cast<uint32>(m_vecNode.size());
	//	}
	//	CNiflectInstanceNode* GetNode(uint32 idx) const
	//	{
	//		return m_vecNode[idx].Get();
	//	}

	//private:
	//	InstanceType* m_base;
	//	CSharedSaveLoad m_delegate;
	//	Niflect::TArray<CSharedInstanceNode> m_vecNode;
	//};

	//static CSharedInstanceNode CreateInstanceNode()
	//{
	//	return Niflect::MakeShared<CNiflectInstanceNode>();
	//}

	//template <typename T>
	//class TSaveLoadAddrBuildinType : public CNiflectSaveLoad
	//{
	//public:
	//	virtual bool Save(CRwNode* rw) const override
	//	{
	//		SetRwValueAs<T>(rw->ToValue(), *static_cast<const T*>(m_base));
	//		return true;
	//	}
	//	virtual bool Load(const CRwNode* rw) const override
	//	{
	//		*static_cast<T*>(m_base) = GetRwValueAs<T>(rw->GetValue());
	//		return true;
	//	}
	//};


	using namespace RwTree;

	class CNiflectType;

	class CNiflectInstanceNode;
	using CSharedInstanceNode = Niflect::TSharedPtr<CNiflectInstanceNode>;

	class CNiflectInstanceNode
	{
	public:
		CNiflectInstanceNode()
			: m_type(NULL)
			, m_base(NULL)
			, m_parent(NULL)
		{

		}
		void Init(CNiflectType* type, InstanceType* base, const Niflect::CString& name = Niflect::CString(), CNiflectInstanceNode* parent = NULL)
		{
			m_type = type;
			m_base = base;
			m_name = name;
			m_parent = parent;
		}
		bool InitAndBuild(CNiflectType* type, InstanceType* base, const Niflect::CString& name = Niflect::CString(), CNiflectInstanceNode* parent = NULL)
		{
			this->Init(type, base, name, parent);
			return this->Build();
		}
		const Niflect::CString& GetName() const
		{
			return m_name;
		}
		CNiflectType* GetType() const
		{
			return m_type;
		}
		InstanceType* GetBase() const
		{
			return m_base;
		}
		CNiflectInstanceNode* GetParent() const
		{
			return m_parent;
		}
		void AddNode(const CSharedInstanceNode& node)
		{
			return m_vecNode.push_back(node);
		}
		uint32 GetNodesCount() const
		{
			return static_cast<uint32>(m_vecNode.size());
		}
		CNiflectInstanceNode* GetNode(uint32 idx) const
		{
			return m_vecNode[idx].Get();
		}
		void Clear()
		{
			m_vecNode.clear();
		}
		NIFLECT_API bool Save(CRwNode* rw) const;
		NIFLECT_API bool Load(const CRwNode* rw) const;
		NIFLECT_API bool Build();

	private:
		friend CNiflectInstanceNode* VisitInstanceNodeByNamePath(CNiflectInstanceNode* root, const Niflect::TArray<Niflect::CString>& vecPath);
		static CNiflectInstanceNode* VisitByNamePathRecurs(CNiflectInstanceNode* parent, const Niflect::TArray<Niflect::CString>& vecPath, uint32 frontIdx)
		{
			if (parent->GetName() == vecPath[frontIdx])
			{
				if (vecPath.size() == ++frontIdx)
					return parent;
				for (auto& it : parent->m_vecNode)
				{
					if (auto found = VisitByNamePathRecurs(it.Get(), vecPath, frontIdx))
						return found;
				}
			}
			return NULL;
		}

	private:
		Niflect::CString m_name;
		CNiflectType* m_type;
		InstanceType* m_base;
		CNiflectInstanceNode* m_parent;
		Niflect::TArray<CSharedInstanceNode> m_vecNode;
	};

	static CSharedInstanceNode CreateInstanceNode()
	{
		return Niflect::MakeShared<CNiflectInstanceNode>();
	}
	inline CNiflectInstanceNode* VisitInstanceNodeByNamePath(CNiflectInstanceNode* root, const Niflect::TArray<Niflect::CString>& vecPath)
	{
		ASSERT(vecPath.size() > 0);
		return CNiflectInstanceNode::VisitByNamePathRecurs(root, vecPath, 0);
	}
	template <typename TTypeHas_GetName_and_GetParent>
	static bool BuildNamePathForTypeHasGetNameGetParent(const TTypeHas_GetName_and_GetParent* node, Niflect::TArray<Niflect::CString>& vecPath)
	{
		auto par = node;
		while (par != NULL)
		{
			vecPath.insert(vecPath.begin(), par->GetName());
			par = par->GetParent();
		}
		return true;
	}
}

namespace Niflect
{
	static void DebugPrintInstanceNodeRecurs(const CNiflectInstanceNode* parent, uint32 depth = 0)
	{
		auto strDep = NiflectUtil::DebugIndentToString(depth);
		auto nodeName = parent->GetName();
		printf("%s%s\n", strDep.c_str(), nodeName.c_str());
		depth++;
		for (uint32 idx = 0; idx < parent->GetNodesCount(); ++idx)
			DebugPrintInstanceNodeRecurs(parent->GetNode(idx), depth);
	}
}