#pragma once
#include "Niflect/NiflectCommon.h"
#include "Niflect/NiflectAddr.h"
#include "Niflect/NiflectNata.h"
#include "Niflect/Serialization/RwTree.h"
#include "Niflect/NiflectInstanceNode.h"

namespace Niflect
{
	using namespace RwTree;

	class CNiflectType;

#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
	class CField
	{
		friend class CNiflectType;
	public:
		CField()
			: m_offset(OFFSET_NONE)
			, m_type(NULL)
			, m_fieldHash(INVALID_HASH)
		{
		}
		void Init(const Niflect::CString& name, const OffsetType& offset, CNiflectType* type, const CSharedNata& nata, const HashInt& fieldHash)
		{
			m_name = name;
			m_offset = offset;
			m_type = type;
			m_nata = nata;
			m_fieldHash = fieldHash;
		}
		const Niflect::CString& GetName() const
		{
			return m_name;
		}
		CNiflectType* GetType() const
		{
			return m_type;
		}
		CNata* GetNata() const
		{
			return m_nata.Get();
		}
		const OffsetType& GetOffset() const
		{
			return m_offset;
		}

	private:
		const InstanceType* GetAddr(const InstanceType* base) const
		{
			return GetOffsetAddr(base, m_offset);
		}
		InstanceType* GetAddr(InstanceType* base) const
		{
			return GetOffsetAddr(base, m_offset);
		}

	public:
		NIFLECT_API bool TypeSaveInstanceToRwNode(const InstanceType* base, CRwNode* rw) const;
		NIFLECT_API bool TypeLoadInstanceFromRwNode(InstanceType* base, const CRwNode* rw) const;
		NIFLECT_API bool TypeBuildInstanceNode(CNiflectInstanceNode* parent, CNiflectInstanceNode* node) const;

	private:
		Niflect::CString m_name;
		CNiflectType* m_type;
		CSharedNata m_nata;
		OffsetType m_offset;
		HashInt m_fieldHash;
	};

	NIFLECT_API HashInt ComputeFieldHash(const Niflect::CString& str);
#endif
}