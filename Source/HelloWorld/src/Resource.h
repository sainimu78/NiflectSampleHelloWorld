#pragma once
#include "Niflect/Base/SharedPtr.h"
#include "Niflect/Base/Map.h"
#include "Niflect/Base/String.h"
#include "Niflect/Base/Array.h"

class CResource
{
public:
	const Niflect::CString* m_id = NULL;
};
using CSharedResource = Niflect::TSharedPtr<CResource>;

class CResourceFactory
{
public:
	CResource* Register(const Niflect::CString& id)
	{
		auto ret = m_mapIdToIdx.insert({ id, static_cast<int>(m_vecRes.size())});
		const auto& idx = ret.first->second;
		if (ret.second)
		{
			auto res = Niflect::MakeShared<CResource>();
			res->m_id = &ret.first->first;
			m_vecRes.push_back(res);
		}
		else
		{
			assert(false);
		}
		return m_vecRes[idx].Get();
	}
	CResource* Find(const Niflect::CString& id) const
	{
		return m_vecRes[m_mapIdToIdx.at(id)].Get();
	}

private:
	Niflect::TMap<Niflect::CString, int> m_mapIdToIdx;
	Niflect::TArray<CSharedResource> m_vecRes;
};

CResourceFactory* GetResourceFactory();
void DestroyResourceFactory();