#pragma once
#include "Niflect/NiflectField.h"

namespace Niflect
{
	class CAccessor;
	using CSharedAccessor = TSharedPtr<CAccessor>;

#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
	class CTypeLayout
	{
	public:
		inline bool AccessorsSaveToRwNode(const InstanceType* base, CRwNode* rw) const
		{
			return this->AccessorsSaveToRwNode(base, CAddrOffset::None, rw);
		}
		inline bool AccessorsLoadFromRwNode(InstanceType* base, const CRwNode* rw) const
		{
			return this->AccessorsLoadFromRwNode(base, CAddrOffset::None, rw);
		}
		NIFLECT_API bool AccessorsSaveToRwNode(const InstanceType* base, const OffsetType& offs, CRwNode* rw) const;
		NIFLECT_API bool AccessorsLoadFromRwNode(InstanceType* base, const OffsetType& offs, const CRwNode* rw) const;
		Niflect::TArrayNif<CNiflectType*> m_vecSection;
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
	class CAccessor
	{
	protected:
		using InstanceType = Niflect::InstanceType;

	public:
		CAccessor()
			: m_type(NULL)
			, m_elemType(NULL)
		{
		}

	public:
		bool SaveToRwNode(const InstanceType* base, const CAddrOffset& offs, CRwNode* rw) const
		{
			return this->SaveInstanceImpl(GetAddrFromBase(base, offs), rw);
		}
		bool LoadFromRwNode(InstanceType* base, const CAddrOffset& offs, const CRwNode* rw) const
		{
			return this->LoadInstanceImpl(GetAddrFromBase(base, offs), rw);
		}

	public:
		void InitType(CNiflectType* type)
		{
			m_type = type;
		}
		void InitElementType(CNiflectType* type)
		{
			m_elemType = type;
		}

	public:
		CNiflectType* GetType() const
		{
			return m_type;
		}
		NIFLECT_API const Niflect::TArray<CField>& GetFields() const;
		NIFLECT_API const CTypeLayout& GetElementLayout() const;

	protected:
		virtual bool SaveInstanceImpl(const InstanceType* base, CRwNode* rw) const = 0;
		virtual bool LoadInstanceImpl(InstanceType* base, const CRwNode* rw) const = 0;

	private:
		inline static const InstanceType* GetAddrFromBase(const InstanceType*& base, const CAddrOffset& addrOffset)
		{
			return static_cast<const char*>(base) + addrOffset.GetOffset();
		}
		inline static InstanceType* GetAddrFromBase(InstanceType*& base, const CAddrOffset& addrOffset)
		{
			return static_cast<char*>(base) + addrOffset.GetOffset();
		}

	private:
		CNiflectType* m_elemType;
		CNiflectType* m_type;
	};
#else
	class CAccessor
	{
	protected:
		using InstanceType = Niflect::InstanceType;

	public:
		CAccessor()
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
			ASSERT(false);
			//m_name = name;
			m_addrOffset.SetOffset(offset);
		}

	public:
		void InitType(CNiflectType* type)
		{
			ASSERT(false);
			m_type = type;
			//ASSERT(m_name.empty());
			ASSERT(m_addrOffset.GetOffset() == CAddrOffset::None);
		}
		void InitForField(const CString& name, const OffsetType& offset)
		{
			ASSERT(false);
			//ASSERT(m_name.empty());
			//m_name = name;
			ASSERT(m_addrOffset.GetOffset() == CAddrOffset::None);
			m_addrOffset.SetOffset(offset);
		}
		void InitForElement()
		{
			ASSERT(false);//
			//ASSERT(m_name.empty());
			//m_name = "reserved_dim";
			ASSERT(m_addrOffset.GetOffset() == CAddrOffset::None);
		}
		void InitElementAccessor(const CSharedAccessor& accessor)
		{
			ASSERT(false);
			//m_elemAccessor = accessor;
		}
		void AddChild(const CSharedAccessor& accessor)
		{
			this->InsertChild(accessor, this->GetChildrenCount());
		}
		void InsertChild(const CSharedAccessor& accessor, uint32 idx)
		{
			ASSERT(false);
			//m_vecChild.insert(m_vecChild.begin() + idx, accessor);
		}

	public:
		void InitType2(CNiflectType* type)
		{
			m_type = type;
			ASSERT(m_addrOffset.GetOffset() == CAddrOffset::None);
		}
		void InitOffset(const OffsetType& offset)
		{
			ASSERT(m_addrOffset.GetOffset() == CAddrOffset::None);
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
		void InsertField(const CField& field, uint32 idx)
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
			ASSERT(false);
			static Niflect::CString s_a;
			return s_a;
		}
		uint32 GetChildrenCount() const
		{
			//return static_cast<uint32>(m_vecChild.size());
			ASSERT(false);
			return 0;
		}
		uint32 GetFieldsCount() const
		{
			return static_cast<uint32>(m_vecField.size());
		}
		const Niflect::TArrayNif<CField>& GetFields() const
		{
			return m_vecField;
		}
		CAccessor* GetChild(uint32 idx) const
		{
			//return m_vecChild[idx].Get();
			ASSERT(false);
			return NULL;
		}
		CAccessor* GetElementAccessor() const
		{
			ASSERT(false);
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
		static void InvokeStaticMemberFunction(InstanceType** const args)
		{
			static_assert(sizeof(TType) == 0, "This function must be specialized for type TType");//仅为避免 Intellisense 的绿线, 实际上只需要声明即可
		}
	};
}