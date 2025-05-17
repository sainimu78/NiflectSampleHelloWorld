#pragma once
#include "Niflect/NiflectField.h"

namespace Niflect
{
	class CNiflectAccessor;
	using CSharedAccessor = TSharedPtr<CNiflectAccessor>;

#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
	class CTypeLayout
	{
	public:
		NIFLECT_API bool AccessorsSaveToRwNode(const InstanceType* base, CRwNode* rw) const;
		NIFLECT_API bool AccessorsLoadFromRwNode(InstanceType* base, const CRwNode* rw) const;
		NIFLECT_API bool AccessorsBuildInstanceNode(CNiflectInstanceNode* node) const;
		Niflect::TArrayNif<CNiflectType*> m_vecSubobject;
	};
#else
	class CTypeLayout
	{
	public:
		bool AccessorsSaveToRwNode(const InstanceType* base, CRwNode* rw) const;
		bool AccessorsLoadFromRwNode(InstanceType* base, const CRwNode* rw) const;
		Niflect::TArrayNif<CSharedAccessor> m_vecAccessor;
	};
#endif

#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
#else
	class CField
	{
		friend class CNiflectType;
	public:
		void Init(const Niflect::CString& name, const CSharedNata& nata)
		{
			m_name = name;
			m_nata = nata;
		}
		void InitAddAccessor(const CSharedAccessor& accessor)
		{
			m_layout.m_vecAccessor.push_back(accessor);
		}
		const Niflect::CString& GetName() const
		{
			return m_name;
		}
		const Niflect::TArrayNif<CSharedAccessor>& GetAccessors() const
		{
			return m_layout.m_vecAccessor;
		}
		bool LayoutSaveToRwNode(const InstanceType* base, CRwNode* rw) const
		{
			return m_layout.AccessorsSaveToRwNode(base, rw);
		}
		bool LayoutLoadFromRwNode(InstanceType* base, const CRwNode* rw) const
		{
			return m_layout.AccessorsLoadFromRwNode(base, rw);
		}

	private:
		Niflect::CString m_name;
		CTypeLayout m_layout;
		CSharedNata m_nata;
	};
#endif

#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
	class CNiflectAccessor
	{
	protected:
		using InstanceType = Niflect::InstanceType;
		using CNiflectInstanceNode = Niflect::CNiflectInstanceNode;

	public:
		CNiflectAccessor()
			: m_ownerType(NULL)
			, m_elemType(NULL)
		{
		}

	public:
		bool SaveToRwNode(const InstanceType* base, CRwNode* rw) const
		{
			return this->SaveInstanceImpl(base, rw);
		}
		bool LoadFromRwNode(InstanceType* base, const CRwNode* rw) const
		{
			return this->LoadInstanceImpl(base, rw);
		}
		bool BuildInstanceNode(CNiflectInstanceNode* node) const
		{
			return this->BuildInstanceNodeImpl(node);
		}

	public:
		void InitOwnerType(CNiflectType* type)
		{
			m_ownerType = type;
		}
		void InitElementType(CNiflectType* type)
		{
			m_elemType = type;
		}

	public:
		CNiflectType* GetOwnerType() const
		{
			return m_ownerType;
		}
		CNiflectType* GetElementType() const
		{
			return m_elemType;
		}

	protected:
		virtual bool SaveInstanceImpl(const InstanceType* base, CRwNode* rw) const = 0;
		virtual bool LoadInstanceImpl(InstanceType* base, const CRwNode* rw) const = 0;
		virtual bool BuildInstanceNodeImpl(CNiflectInstanceNode* node) const { return true; }

	private:
		CNiflectType* m_elemType;
		CNiflectType* m_ownerType;
	};
#else
	class CNiflectAccessor
	{
	protected:
		using InstanceType = Niflect::InstanceType;

	public:
		CNiflectAccessor()
			: m_type(NULL)
		{
		}

	public:
		bool SaveToRwNode(const InstanceType* base, CRwNode* rw) const
		{
			return this->SaveInstanceImpl(this->GetAddrFromBase(base), rw);
		}
		bool LoadFromRwNode(InstanceType* base, const CRwNode* rw) const
		{
			return this->LoadInstanceImpl(this->GetAddrFromBase(base), rw);
		}

	public:
		void InitMemberMeta(const CString& name, const OffsetType& offset)
		{
			NIFLECT_ASSERT(false);
			//m_name = name;
			m_addrOffset.SetOffset(offset);
		}

	public:
		void InitType(CNiflectType* type)
		{
			NIFLECT_ASSERT(false);
			m_type = type;
			//NIFLECT_ASSERT(m_name.empty());
			NIFLECT_ASSERT(m_addrOffset.GetOffset() == OFFSET_NONE);
		}
		void InitForField(const CString& name, const OffsetType& offset)
		{
			NIFLECT_ASSERT(false);
			//NIFLECT_ASSERT(m_name.empty());
			//m_name = name;
			NIFLECT_ASSERT(m_addrOffset.GetOffset() == OFFSET_NONE);
			m_addrOffset.SetOffset(offset);
		}
		void InitForElement()
		{
			NIFLECT_ASSERT(false);//
			//NIFLECT_ASSERT(m_name.empty());
			//m_name = "reserved_dim";
			NIFLECT_ASSERT(m_addrOffset.GetOffset() == OFFSET_NONE);
		}
		void InitElementAccessor(const CSharedAccessor& accessor)
		{
			NIFLECT_ASSERT(false);
			//m_elemAccessor = accessor;
		}
		void AddChild(const CSharedAccessor& accessor)
		{
			this->InsertChild(accessor, this->GetChildrenCount());
		}
		void InsertChild(const CSharedAccessor& accessor, NifUint32 idx)
		{
			NIFLECT_ASSERT(false);
			//m_vecChild.insert(m_vecChild.begin() + idx, accessor);
		}

	public:
		void InitType2(CNiflectType* type)
		{
			m_type = type;
			NIFLECT_ASSERT(m_addrOffset.GetOffset() == OFFSET_NONE);
		}
		void InitOffset(const OffsetType& offset)
		{
			NIFLECT_ASSERT(m_addrOffset.GetOffset() == OFFSET_NONE);
			m_addrOffset.SetOffset(offset);
		}
		void InitAddField(const CField& field)
		{
			this->InsertField(field, this->GetFieldsCount());
		}
		void InitElementLayout(const CTypeLayout& layout)
		{
			m_elemLayout = layout;
		}

	private:
		void InsertField(const CField& field, NifUint32 idx)
		{
			m_vecField.insert(m_vecField.begin() + idx, field);
		}

	public:
		CNiflectType* GetType() const
		{
			return m_type;
		}
		const CString& GetName() const
		{
			//return m_name;
			NIFLECT_ASSERT(false);
			static Niflect::CString s_a;
			return s_a;
		}
		NifUint32 GetChildrenCount() const
		{
			//return static_cast<NifUint32>(m_vecChild.size());
			NIFLECT_ASSERT(false);
			return 0;
		}
		NifUint32 GetFieldsCount() const
		{
			return static_cast<NifUint32>(m_vecField.size());
		}
		const Niflect::TArrayNif<CField>& GetFields() const
		{
			return m_vecField;
		}
		CNiflectAccessor* GetChild(NifUint32 idx) const
		{
			//return m_vecChild[idx].Get();
			NIFLECT_ASSERT(false);
			return NULL;
		}
		CNiflectAccessor* GetElementAccessor() const
		{
			NIFLECT_ASSERT(false);
			//return m_elemAccessor.Get();
			return NULL;
		}
		const CTypeLayout& GetElementLayout() const
		{
			return m_elemLayout;
		}

	protected:
		virtual bool SaveInstanceImpl(const InstanceType* base, CRwNode* rw) const = 0;
		virtual bool LoadInstanceImpl(InstanceType* base, const CRwNode* rw) const = 0;

	private:
		inline const InstanceType* GetAddrFromBase(const InstanceType*& base) const
		{
			return static_cast<const char*>(base) + m_addrOffset.GetOffset();
		}
		inline InstanceType* GetAddrFromBase(InstanceType*& base) const
		{
			return static_cast<char*>(base) + m_addrOffset.GetOffset();
		}

	private:
		CAddrOffset m_addrOffset;
		//CString m_name;
		TArrayNif<CField> m_vecField;
		//CSharedAccessor m_elemAccessor;
		CTypeLayout m_elemLayout;
		CNiflectType* m_type;
	};
#endif

#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
#else
	inline bool CTypeLayout::AccessorsSaveToRwNode(const InstanceType* base, CRwNode* rw) const
	{
		for (auto& it : m_vecAccessor)
		{
			if (!it->SaveToRwNode(base, rw))
				return false;
		}
		return true;
	}
	inline bool CTypeLayout::AccessorsLoadFromRwNode(InstanceType* base, const CRwNode* rw) const
	{
		for (auto& it : m_vecAccessor)
		{
			if (!it->LoadFromRwNode(base, rw))
				return false;
		}
		return true;
	}
#endif

	class CTypeBody
	{
	public:
#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
		template <typename TType>
		static void BuildTypeMeta(CNiflectType* type0)
		{
			static_assert(sizeof(TType) == 0, "This function must be specialized for type TType");//仅为避免 Intellisense 的绿线, 实际上只需要声明即可
		}
#else
		template <typename TType>
		static CSharedAccessor CreateTypeAccessor()
		{
			static_assert(sizeof(TType) == 0, "This function must be specialized for type TType");//仅为避免 Intellisense 的绿线, 实际上只需要声明即可
			return NULL;
		}
#endif
		template <typename TType, int MethodIndex>
		static void InvokeMethod(InstanceType* base, InstanceType** const args)
		{
			static_assert(sizeof(TType) == 0, "This function must be specialized for type TType");//仅为避免 Intellisense 的绿线, 实际上只需要声明即可
		}
		template <typename TType, int MethodIndex>
		static void InvokeConstructor(InstanceType* base, InstanceType** const args)
		{
			static_assert(sizeof(TType) == 0, "This function must be specialized for type TType");//仅为避免 Intellisense 的绿线, 实际上只需要声明即可
		}
		template <typename TType, int FuncIndex>
		static void InvokeStaticMethod(InstanceType** const args)
		{
			static_assert(sizeof(TType) == 0, "This function must be specialized for type TType");//仅为避免 Intellisense 的绿线, 实际上只需要声明即可
		}
	};
}