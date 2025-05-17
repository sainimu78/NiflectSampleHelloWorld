#pragma once
#include "Niflect/Base/SharedPtr.h"
#include "Niflect/Util/DebugUtil.h"
#include <cstring>

namespace RwTree
{
	enum class ERwValueType : Niflect::NifUint8
	{
		None,
		Bool,
		Int8,
		Int16,
		Int32,
		Int64,
		Uint8,
		Uint16,
		Uint32,
		Uint64,
		Float,
		Double,
		String,
		RawData,
	};

	typedef Niflect::CString CRwRawData;//用字符串是为顺便避免String类型的转换

	class CRwValue
	{
	public:
		CRwValue(CRwRawData& data, ERwValueType& type)
			: m_data(data)
			, m_type(type)
		{
		}
		const ERwValueType& GetType() const
		{
			return m_type;
		}

		template <typename T>
		const T& GetAs() const;
		template <typename T>
		void SetAs(const T& val);

	public:
		void SetBool(const bool& val)
		{
			this->SetBuiltInValue<ERwValueType::Bool>(val);
		}
		const bool& GetBool() const
		{
			return this->GetBuiltInValue<ERwValueType::Bool, bool>();
		}
		void SetInt8(const Niflect::NifInt8& val)
		{
			this->SetBuiltInValue<ERwValueType::Int8>(val);
		}
		const Niflect::NifInt8& GetInt8() const
		{
			return this->GetBuiltInValue<ERwValueType::Int8, Niflect::NifInt8>();
		}
		void SetInt16(const Niflect::NifInt16& val)
		{
			this->SetBuiltInValue<ERwValueType::Int16>(val);
		}
		const Niflect::NifInt16& GetInt16() const
		{
			return this->GetBuiltInValue<ERwValueType::Int16, Niflect::NifInt16>();
		}
		void SetInt32(const Niflect::NifInt32& val)
		{
			this->SetBuiltInValue<ERwValueType::Int32>(val);
		}
		const Niflect::NifInt32& GetInt32() const
		{
			return this->GetBuiltInValue<ERwValueType::Int32, Niflect::NifInt32>();
		}
		void SetInt64(const Niflect::NifInt64& val)
		{
			this->SetBuiltInValue<ERwValueType::Int64>(val);
		}
		const Niflect::NifInt64& GetInt64() const
		{
			return this->GetBuiltInValue<ERwValueType::Int64, Niflect::NifInt64>();
		}
		void SetUint8(const Niflect::NifUint8& val)
		{
			this->SetBuiltInValue<ERwValueType::Uint8>(val);
		}
		const Niflect::NifUint8& GetUint8() const
		{
			return this->GetBuiltInValue<ERwValueType::Uint8, Niflect::NifUint8>();
		}
		void SetUint16(const Niflect::NifUint16& val)
		{
			this->SetBuiltInValue<ERwValueType::Uint16>(val);
		}
		const Niflect::NifUint16& GetUint16() const
		{
			return this->GetBuiltInValue<ERwValueType::Uint16, Niflect::NifUint16>();
		}
		void SetUint32(const Niflect::NifUint32& val)
		{
			this->SetBuiltInValue<ERwValueType::Uint32>(val);
		}
		const Niflect::NifUint32& GetUint32() const
		{
			return this->GetBuiltInValue<ERwValueType::Uint32, Niflect::NifUint32>();
		}
		void SetUint64(const Niflect::NifUint64& val)
		{
			this->SetBuiltInValue<ERwValueType::Uint64>(val);
		}
		const Niflect::NifUint64& GetUint64() const
		{
			return this->GetBuiltInValue<ERwValueType::Uint64, Niflect::NifUint64>();
		}
		void SetFloat(const float& val)
		{
			this->SetBuiltInValue<ERwValueType::Float>(val);
		}
		const float& GetFloat() const
		{
			return this->GetBuiltInValue<ERwValueType::Float, float>();
		}
		void SetDouble(const double& val)
		{
			this->SetBuiltInValue<ERwValueType::Double>(val);
		}
		const double& GetDouble() const
		{
			return this->GetBuiltInValue<ERwValueType::Double, double>();
		}
		void SetString(const Niflect::CString& val)
		{
			m_type = ERwValueType::String;
			m_data = val;
		}
		const Niflect::CString& GetString() const
		{
			NIFLECT_ASSERT(m_type == ERwValueType::String);
			return m_data;
		}

	public:
		void SetRawData(const CRwRawData& val)
		{
			m_type = ERwValueType::RawData;
			m_data = val;
		}
		void SetRawData(const void* src, Niflect::NifUint32 size)
		{
			m_type = ERwValueType::RawData;
			m_data.resize(size);
			memcpy(&m_data[0], src, size);
		}
		const CRwRawData& GetRawData() const
		{
			NIFLECT_ASSERT(m_type == ERwValueType::RawData);
			return m_data;
		}

	private:
		template <ERwValueType TEnumType, typename TValue>
		inline void SetBuiltInValue(const TValue& val)
		{
			m_type = TEnumType;
			auto cnt = sizeof(val);
			m_data.resize(cnt);
			memcpy(&m_data[0], &val, cnt);
		}
		template <ERwValueType TEnumType, typename TValue>
		inline const TValue& GetBuiltInValue() const
		{
			NIFLECT_ASSERT(m_type == TEnumType);
			return *reinterpret_cast<TValue*>(&m_data[0]);
		}

	private:
		CRwRawData& m_data;
		ERwValueType& m_type;
	};
	using CSharedRwValue = Niflect::TSharedPtr<CRwValue>;

	template <>
	inline void CRwValue::SetAs<bool>(const bool& val)
	{
		return this->SetBool(val);
	}
	template <>
	inline const bool& CRwValue::GetAs<bool>() const
	{
		return this->GetBool();
	}
	template <>
	inline void CRwValue::SetAs<Niflect::NifInt8>(const Niflect::NifInt8& val)
	{
		return this->SetInt8(val);
	}
	template <>
	inline const Niflect::NifInt8& CRwValue::GetAs<Niflect::NifInt8>() const
	{
		return this->GetInt8();
	}
	template <>
	inline void CRwValue::SetAs<Niflect::NifInt16>(const Niflect::NifInt16& val)
	{
		return this->SetInt16(val);
	}
	template <>
	inline const Niflect::NifInt16& CRwValue::GetAs<Niflect::NifInt16>() const
	{
		return this->GetInt16();
	}
	template <>
	inline void CRwValue::SetAs<Niflect::NifInt32>(const Niflect::NifInt32& val)
	{
		return this->SetInt32(val);
	}
	template <>
	inline const Niflect::NifInt32& CRwValue::GetAs<Niflect::NifInt32>() const
	{
		return this->GetInt32();
	}
	template <>
	inline void CRwValue::SetAs<Niflect::NifInt64>(const Niflect::NifInt64& val)
	{
		return this->SetInt64(val);
	}
	template <>
	inline const Niflect::NifInt64& CRwValue::GetAs<Niflect::NifInt64>() const
	{
		return this->GetInt64();
	}
	template <>
	inline void CRwValue::SetAs<Niflect::NifUint8>(const Niflect::NifUint8& val)
	{
		return this->SetUint8(val);
	}
	template <>
	inline const Niflect::NifUint8& CRwValue::GetAs<Niflect::NifUint8>() const
	{
		return this->GetUint8();
	}
	template <>
	inline void CRwValue::SetAs<Niflect::NifUint16>(const Niflect::NifUint16& val)
	{
		return this->SetUint16(val);
	}
	template <>
	inline const Niflect::NifUint16& CRwValue::GetAs<Niflect::NifUint16>() const
	{
		return this->GetUint16();
	}
	template <>
	inline void CRwValue::SetAs<Niflect::NifUint32>(const Niflect::NifUint32& val)
	{
		return this->SetUint32(val);
	}
	template <>
	inline const Niflect::NifUint32& CRwValue::GetAs<Niflect::NifUint32>() const
	{
		return this->GetUint32();
	}
	template <>
	inline void CRwValue::SetAs<Niflect::NifUint64>(const Niflect::NifUint64& val)
	{
		return this->SetUint64(val);
	}
	template <>
	inline const Niflect::NifUint64& CRwValue::GetAs<Niflect::NifUint64>() const
	{
		return this->GetUint64();
	}
	template <>
	inline void CRwValue::SetAs<float>(const float& val)
	{
		return this->SetFloat(val);
	}
	template <>
	inline const float& CRwValue::GetAs<float>() const
	{
		return this->GetFloat();
	}
	template <>
	inline void CRwValue::SetAs<double>(const double& val)
	{
		return this->SetDouble(val);
	}
	template <>
	inline const double& CRwValue::GetAs<double>() const
	{
		return this->GetDouble();
	}
	template <>
	inline void CRwValue::SetAs<Niflect::CString>(const Niflect::CString& val)
	{
		return this->SetString(val);
	}
	template <>
	inline const Niflect::CString& CRwValue::GetAs<Niflect::CString>() const
	{
		return this->GetString();
	}

	class CRwNode;
	using CSharedRwNode = Niflect::TSharedPtr<CRwNode>;

	class CRwArray
	{
	public:
		CRwArray(CRwRawData& data, Niflect::TArrayNif<CSharedRwNode>& vecItem)
			: m_data(data)
			, m_vecItem(vecItem)
		{
		}
		void AddItem(const CSharedRwNode& item)
		{
			this->InsertItem(item, this->GetItemsCount());
		}
		void InsertItem(const CSharedRwNode& item, Niflect::NifUint32 idx)
		{
			m_vecItem.insert(m_vecItem.begin() + idx, item);
		}
		CRwNode* AddItemNode()
		{
			return InternalAddItemNode(this);
		}
		CRwValue* AddItemValue()
		{
			return InternalAddItemValue(this);
		}
		CRwArray* AddItemArray()
		{
			return InternalAddItemArray(this);
		}
		void AddFloat(float val)
		{
			auto rwValue = this->AddItemValue();
			rwValue->SetFloat(val);
		}
		void AddItemBool(bool value)
		{
			auto rwValue = this->AddItemValue();
			rwValue->SetBool(value);
		}
		void AddItemInt32(Niflect::NifInt32 value)
		{
			auto rwValue = this->AddItemValue();
			rwValue->SetInt32(value);
		}
		void AddItemFloat(float value)
		{
			auto rwValue = this->AddItemValue();
			rwValue->SetFloat(value);
		}
		void AddItemString(const Niflect::CString& value)
		{
			auto rwValue = this->AddItemValue();
			rwValue->SetString(value);
		}
		void SetItem(const CSharedRwNode& item, Niflect::NifUint32 idx)
		{
			m_vecItem[idx] = item;
		}
		CRwNode* GetItem(Niflect::NifUint32 idx) const
		{
			return m_vecItem[idx].Get();
		}
		Niflect::NifUint32 GetItemsCount() const
		{
			return static_cast<Niflect::NifUint32>(m_vecItem.size());
		}
		void Resize(Niflect::NifUint32 cnt)
		{
			m_vecItem.resize(cnt);
		}
		void DeleteItem(Niflect::NifUint32 idx)
		{
			m_vecItem.erase(m_vecItem.begin() + idx);
		}
		void DeleteLastItem()
		{
			m_vecItem.erase(m_vecItem.begin() + (m_vecItem.size() - 1));
		}

	private:
		static CRwNode* InternalAddItemNode(CRwArray* rwArray);
		static CRwValue* InternalAddItemValue(CRwArray* rwArray);
		static CRwArray* InternalAddItemArray(CRwArray* rwArray);

	private:
		CRwRawData& m_data;
		Niflect::TArrayNif<CSharedRwNode>& m_vecItem;
	};
	using CSharedRwArray = Niflect::TSharedPtr<CRwArray>;

	class CRwNode
	{
	public:
		CRwNode()
			: m_valueType(ERwValueType::None)
		{
		}
		void SetName(const Niflect::CString& name)
		{
			m_name = name;
		}
		const Niflect::CString& GetName() const
		{
			return m_name;
		}
		bool IsValue() const
		{
			return m_rwValue != NULL;
		}
		bool IsArray() const
		{
			return m_rwArray != NULL;
		}
		void AddNode(const CSharedRwNode& node)
		{
			this->InsertNode(node, this->GetNodesCount());
		}
		void InsertNode(const CSharedRwNode& node, Niflect::NifUint32 idx)
		{
			m_vecNode.insert(m_vecNode.begin() + idx, node);
		}
		CRwNode* FindNode(const Niflect::CString& name) const
		{
			for (auto& it : m_vecNode)
			{
				if (it->GetName() == name)
					return it.Get();
			}
			return NULL;
		}
		void Resize(Niflect::NifUint32 cnt)
		{
			m_vecNode.resize(cnt);
		}
		void Delete(Niflect::NifUint32 idx)
		{
			m_vecNode.erase(m_vecNode.begin() + idx);
		}
		void ResetType()
		{
			m_rwValue = NULL;
			m_rwArray = NULL;
		}
		void ClearNodes()
		{
			m_vecNode.clear();
		}
		void SetNode(const CSharedRwNode& rwNode, Niflect::NifUint32 idx)
		{
			m_vecNode[idx] = rwNode;
		}
		CRwNode* GetNode(Niflect::NifUint32 idx) const
		{
			return m_vecNode[idx].Get();
		}
		CSharedRwNode GetSharedNode(Niflect::NifUint32 idx) const
		{
			return m_vecNode[idx];
		}
		Niflect::NifUint32 GetNodesCount() const
		{
			return static_cast<Niflect::NifUint32>(m_vecNode.size());
		}
		CRwValue* ToValue()
		{
			if (m_rwValue == NULL)
			{
				NIFLECT_ASSERT(m_rwArray == NULL);
				m_rwArray = NULL;
				m_rwValue = Niflect::MakeShared<CRwValue>(m_data, m_valueType);
			}
			return m_rwValue.Get();
		}
		CRwValue* GetValue() const
		{
			return m_rwValue.Get();
		}
		CRwArray* ToArray()
		{
			if (m_rwArray == NULL)
			{
				NIFLECT_ASSERT(m_rwValue == NULL);
				m_rwValue = NULL;
				m_rwArray = Niflect::MakeShared<CRwArray>(m_data, m_vecNode);
			}
			return m_rwArray.Get();
		}
		CRwArray* GetArray() const
		{
			return m_rwArray.Get();
		}
		const CRwRawData& GetData() const
		{
			return m_data;
		}

	//public:
	//	// 在 CRwNode 类的 public 部分添加以下声明
	//	bool Equals(const CRwNode* other) const
	//	{

	//		if (this == other)
	//			return true;
	//		if (!other)
	//			return false;

	//		// 比较节点名称
	//		if (m_name != other->m_name)
	//			return false;

	//		// 检查类型一致性（值节点/数组节点）
	//		bool thisIsValue = IsValue();
	//		bool otherIsValue = other->IsValue();
	//		bool thisIsArray = IsArray();
	//		bool otherIsArray = other->IsArray();

	//		if (thisIsValue != otherIsValue || thisIsArray != otherIsArray)
	//			return false;

	//		// 值节点比较
	//		if (thisIsValue) {
	//			if (m_valueType != other->m_valueType)
	//				return false;
	//			return CompareValueData(other);
	//		}

	//		// 数组/普通节点比较子节点
	//		if (m_vecNode.size() != other->m_vecNode.size())
	//			return false;

	//		for (size_t i = 0; i < m_vecNode.size(); ++i) {
	//			const auto& thisNode = m_vecNode[i];
	//			const auto& otherNode = other->m_vecNode[i];

	//			// 处理空指针情况
	//			if (!thisNode || !otherNode) {
	//				if (thisNode || otherNode) // 其中一个非空
	//					return false;
	//				else
	//					continue; // 都为空则继续
	//			}

	//			if (!thisNode->Equals(otherNode.Get()))
	//				return false;
	//		}

	//		return true;
	//	}

	//private:
	//	// 在 CRwNode 类的 private 部分添加以下辅助函数声明
	//	bool CompareValueData(const CRwNode* other) const
	//	{
	//		// 确保类型一致（调用前已检查）
	//		auto thisValue = GetValue();
	//		auto otherValue = other->GetValue();

	//		// 检查数据长度一致性
	//		if (m_data.size() != other->m_data.size())
	//			return false;

	//		// 根据类型进行数据比较
	//		switch (m_valueType) {
	//		case ERwValueType::String:
	//			return m_data == other->m_data;

	//		case ERwValueType::RawData:
	//			return memcmp(m_data.data(), other->m_data.data(), m_data.size()) == 0;

	//			// 基本类型使用 CRwValue 的 Get 方法精确比较
	//		case ERwValueType::Bool:
	//			return thisValue->GetBool() == otherValue->GetBool();
	//		case ERwValueType::Int8:
	//			return thisValue->GetInt8() == otherValue->GetInt8();
	//		case ERwValueType::Int16:
	//			return thisValue->GetInt16() == otherValue->GetInt16();
	//		case ERwValueType::Int32:
	//			return thisValue->GetInt32() == otherValue->GetInt32();
	//		case ERwValueType::Int64:
	//			return thisValue->GetInt64() == otherValue->GetInt64();
	//		case ERwValueType::Uint8:
	//			return thisValue->GetUint8() == otherValue->GetUint8();
	//		case ERwValueType::Uint16:
	//			return thisValue->GetUint16() == otherValue->GetUint16();
	//		case ERwValueType::Uint32:
	//			return thisValue->GetUint32() == otherValue->GetUint32();
	//		case ERwValueType::Uint64:
	//			return thisValue->GetUint64() == otherValue->GetUint64();
	//		case ERwValueType::Float:
	//			return thisValue->GetFloat() == otherValue->GetFloat();
	//		case ERwValueType::Double:
	//			return thisValue->GetDouble() == otherValue->GetDouble();

	//		default:
	//			return false; // 未知类型
	//		}
	//	}
		//bool IsUnused() const
		//{
		//	return
		//		m_name.empty() &&            // 未设置名称
		//		m_vecNode.empty() &&         // 没有子节点
		//		m_data.empty() &&            // 数据缓冲区为空
		//		m_valueType == ERwValueType::None && // 未指定类型
		//		!m_rwValue &&               // 未转换为值节点
		//		!m_rwArray;                 // 未转换为数组节点
		//}

	private:
		Niflect::CString m_name;
		ERwValueType m_valueType;
		Niflect::TArrayNif<CSharedRwNode> m_vecNode;
		CRwRawData m_data;
		CSharedRwValue m_rwValue;
		CSharedRwArray m_rwArray;
	};
	static CSharedRwNode CreateRwNode()
	{
		return Niflect::MakeShared<CRwNode>();
	}
	inline CRwNode* CRwArray::InternalAddItemNode(CRwArray* rwArray)
	{
		auto rwNode = CreateRwNode();
		rwArray->AddItem(rwNode);
		return rwNode.Get();
	}
	inline CRwValue* CRwArray::InternalAddItemValue(CRwArray* rwArray)
	{
		auto rwNode = CreateRwNode();
		rwArray->AddItem(rwNode);
		return rwNode->ToValue();
	}
	inline CRwArray* CRwArray::InternalAddItemArray(CRwArray* rwArray)
	{
		auto rwNode = CreateRwNode();
		rwArray->AddItem(rwNode);
		return rwNode->ToArray();
	}
}

namespace RwTree
{
	static CRwNode* FindRwNode(const CRwNode* rwParent, const Niflect::CString& name)
	{
		if (rwParent != NULL)
			return rwParent->FindNode(name);
		return NULL;
	}
	static CRwArray* FindRwArray(const CRwNode* rwParent, const Niflect::CString& name)
	{
		if (auto rwNode = FindRwNode(rwParent, name))
			return rwNode->ToArray();
		return NULL;
	}
	static CRwValue* FindRwValue(const CRwNode* rwParent, const Niflect::CString& name)
	{
		if (auto rwNode = FindRwNode(rwParent, name))
			return rwNode->ToValue();
		return NULL;
	}

	template <typename TValue>
	static TValue GetRwValueAs(const CRwValue* rwValue, TValue defaultValue = TValue())
	{
		if (rwValue != NULL)
			return rwValue->GetAs<TValue>();
		return defaultValue;
	}

	template <typename TValue>
	static void SetRwValueAs(CRwValue* rwValue, TValue value)
	{
		if (rwValue != NULL)
			rwValue->SetAs<TValue>(value);
	}

	template <typename TValue>
	static TValue FindRwValueAs(const CRwNode* rwParent, const Niflect::CString& name, const TValue& defaultValue = TValue())
	{
		auto value = defaultValue;
		if (auto rwValue = FindRwValue(rwParent, name))
			value = rwValue->GetAs<TValue>();
		return value;
	}
	static bool FindRwBool(const CRwNode* rwParent, const Niflect::CString& name, bool defaultValue = false)
	{
		return FindRwValueAs<bool>(rwParent, name, defaultValue);
	}
	static Niflect::NifInt8 FindRwInt8(const CRwNode* rwParent, const Niflect::CString& name, Niflect::NifInt8 defaultValue = 0)
	{
		return FindRwValueAs<Niflect::NifInt8>(rwParent, name, defaultValue);
	}
	static Niflect::NifInt16 FindRwInt16(const CRwNode* rwParent, const Niflect::CString& name, Niflect::NifInt16 defaultValue = 0)
	{
		return FindRwValueAs<Niflect::NifInt16>(rwParent, name, defaultValue);
	}
	static Niflect::NifInt32 FindRwInt32(const CRwNode* rwParent, const Niflect::CString& name, Niflect::NifInt32 defaultValue = 0)
	{
		return FindRwValueAs<Niflect::NifInt32>(rwParent, name, defaultValue);
	}
	static Niflect::NifInt64 FindRwInt64(const CRwNode* rwParent, const Niflect::CString& name, Niflect::NifInt64 defaultValue = 0)
	{
		return FindRwValueAs<Niflect::NifInt64>(rwParent, name, defaultValue);
	}
	static Niflect::NifUint8 FindRwUint8(const CRwNode* rwParent, const Niflect::CString& name, Niflect::NifUint8 defaultValue = 0u)
	{
		return FindRwValueAs<Niflect::NifUint8>(rwParent, name, defaultValue);
	}
	static Niflect::NifUint16 FindRwUint16(const CRwNode* rwParent, const Niflect::CString& name, Niflect::NifUint16 defaultValue = 0u)
	{
		return FindRwValueAs<Niflect::NifUint16>(rwParent, name, defaultValue);
	}
	static Niflect::NifUint32 FindRwUint32(const CRwNode* rwParent, const Niflect::CString& name, Niflect::NifUint32 defaultValue = 0u)
	{
		return FindRwValueAs<Niflect::NifUint32>(rwParent, name, defaultValue);
	}
	static Niflect::NifUint64 FindRwUint64(const CRwNode* rwParent, const Niflect::CString& name, Niflect::NifUint64 defaultValue = 0u)
	{
		return FindRwValueAs<Niflect::NifUint64>(rwParent, name, defaultValue);
	}
	static float FindRwFloat(const CRwNode* rwParent, const Niflect::CString& name, float defaultValue = 0.0f)
	{
		return FindRwValueAs<float>(rwParent, name, defaultValue);
	}
	static Niflect::CString FindRwString(const CRwNode* rwParent, const Niflect::CString& name, const Niflect::CString& defaultValue = Niflect::CString())
	{
		return FindRwValueAs<Niflect::CString>(rwParent, name, defaultValue);
	}
	static CRwRawData FindRwRawData(const CRwNode* rwParent, const Niflect::CString& name)
	{
		if (auto rwValue = FindRwValue(rwParent, name))
			return rwValue->GetRawData();
		return CRwRawData();
	}
	static Niflect::NifUint32 GetRwNodesCount(const CRwNode* rwParent)
	{
		Niflect::NifUint32 count = 0;
		if (rwParent != NULL)
			count = rwParent->GetNodesCount();
		return count;
	}
	static CRwNode* GetRwNode(const CRwNode* rwParent, Niflect::NifUint32 idx)
	{
		CRwNode* node = NULL;
		if (rwParent != NULL)
			node = rwParent->GetNode(idx);
		return node;
	}
	static CRwArray* ToRwArray(CRwNode* rwNode)
	{
		if (rwNode != NULL)
			return rwNode->ToArray();
		return NULL;
	}
	static CRwValue* ToRwValue(CRwNode* rwNode)
	{
		if (rwNode != NULL)
			return rwNode->ToValue();
		return NULL;
	}
	static Niflect::NifUint32 GetRwItemsCount(const CRwArray* rwArray)
	{
		Niflect::NifUint32 count = 0;
		if (rwArray != NULL)
			count = rwArray->GetItemsCount();
		return count;
	}
	static bool AddExistingRwNode(CRwNode* rwParent, const Niflect::CString& name, const CSharedRwNode& shared, Niflect::NifUint32* insertedIndex = NULL)
	{
		if (rwParent != NULL)
		{
			NIFLECT_ASSERT(shared->GetName().empty());
			shared->SetName(name);
			if (insertedIndex != NULL)
				*insertedIndex = rwParent->GetNodesCount();
			rwParent->AddNode(shared);
			return true;
		}
		return false;
	}
	static CRwNode* AddRwNode(CRwNode* rwParent, const Niflect::CString& name, Niflect::NifUint32* insertedIndex = NULL)
	{
		auto shared = CreateRwNode();
		Niflect::NifUint32 idx = Niflect::NifInvalidIndex;
		if (AddExistingRwNode(rwParent, name, shared, &idx))
		{
			if (insertedIndex != NULL)
				*insertedIndex = idx;
			return shared.Get();
		}
		return NULL;
	}
	static CRwArray* AddRwArray(CRwNode* rwParent, const Niflect::CString& name, Niflect::NifUint32* insertedIndex = NULL)
	{
		return AddRwNode(rwParent, name, insertedIndex)->ToArray();
	}
	static CRwValue* AddRwValue(CRwNode* rwParent, const Niflect::CString& name, Niflect::NifUint32* insertedIndex = NULL)
	{
		return AddRwNode(rwParent, name, insertedIndex)->ToValue();
	}
	template <typename TValue>
	static void AddRwValueAs(CRwNode* rwParent, const Niflect::CString& name, const TValue& value)
	{
		if (auto rwValue = AddRwValue(rwParent, name))
			rwValue->SetAs<TValue>(value);
	}
	static void AddRwBool(CRwNode* rwParent, const Niflect::CString& name, bool value)
	{
		AddRwValueAs<bool>(rwParent, name, value);
	}
	static void AddRwInt8(CRwNode* rwParent, const Niflect::CString& name, Niflect::NifInt8 value)
	{
		AddRwValueAs<Niflect::NifInt8>(rwParent, name, value);
	}
	static void AddRwInt16(CRwNode* rwParent, const Niflect::CString& name, Niflect::NifInt16 value)
	{
		AddRwValueAs<Niflect::NifInt16>(rwParent, name, value);
	}
	static void AddRwInt32(CRwNode* rwParent, const Niflect::CString& name, Niflect::NifInt32 value)
	{
		AddRwValueAs<Niflect::NifInt32>(rwParent, name, value);
	}
	static void AddRwInt64(CRwNode* rwParent, const Niflect::CString& name, Niflect::NifInt64 value)
	{
		AddRwValueAs<Niflect::NifInt64>(rwParent, name, value);
	}
	static void AddRwUint8(CRwNode* rwParent, const Niflect::CString& name, Niflect::NifUint8 value)
	{
		AddRwValueAs<Niflect::NifUint8>(rwParent, name, value);
	}
	static void AddRwUint16(CRwNode* rwParent, const Niflect::CString& name, Niflect::NifUint16 value)
	{
		AddRwValueAs<Niflect::NifUint16>(rwParent, name, value);
	}
	static void AddRwUint32(CRwNode* rwParent, const Niflect::CString& name, Niflect::NifUint32 value)
	{
		AddRwValueAs<Niflect::NifUint32>(rwParent, name, value);
	}
	static void AddRwUint64(CRwNode* rwParent, const Niflect::CString& name, Niflect::NifUint64 value)
	{
		AddRwValueAs<Niflect::NifUint64>(rwParent, name, value);
	}
	static void AddRwFloat(CRwNode* rwParent, const Niflect::CString& name, float value)
	{
		AddRwValueAs<float>(rwParent, name, value);
	}
	static void AddRwDouble(CRwNode* rwParent, const Niflect::CString& name, double value)
	{
		AddRwValueAs<double>(rwParent, name, value);
	}
	static void AddRwString(CRwNode* rwParent, const Niflect::CString& name, const Niflect::CString& value)
	{
		AddRwValueAs<Niflect::CString>(rwParent, name, value);
	}
	static void AddRwRawData(CRwNode* rwParent, const Niflect::CString& name, const void* data, Niflect::NifUint32 size)
	{
		if (auto rwValue = AddRwValue(rwParent, name))
			rwValue->SetRawData(data, size);
	}
}

namespace RwTree
{
	static void DebugBuildSingleValue(CRwNode* root)
	{
		auto value = ToRwValue(root);
		value->SetFloat(1.23f);
	}
	static void DebugBuildSingleArray(CRwNode* root)
	{
		auto rwArray = ToRwArray(root);
		rwArray->AddFloat(1.23f);
		rwArray->AddFloat(4.56f);
	}
	static void DebugBuildStructure(CRwNode* root)
	{
		AddRwFloat(root, "Nihao", 1.23f);
		{
			auto node0 = AddRwNode(root, "Haike");
			AddRwFloat(node0, "Bucuo", 4.56f);
			AddRwFloat(node0, "Shima", 7.89f);
			AddRwString(node0, "Zhende", "Some Text");
		}
		{
			auto rwArray = AddRwArray(root, "Buxin");
			rwArray->AddFloat(2.34f);
			rwArray->AddFloat(2.56f);
		}
		{
			auto rwArray = AddRwArray(root, "Shide");
			{
				auto node1 = rwArray->AddItemNode();
				AddRwFloat(node1, "My Value 0", 3.12f);
			}
			{
				auto node1 = rwArray->AddItemNode();
				AddRwBool(node1, "My Value 1", false);
			}
			{
				auto node1 = rwArray->AddItemNode();
				AddRwBool(node1, "My Value 2", true);
			}
			{
				auto node1 = rwArray->AddItemNode();
				AddRwString(node1, "My Value 3", "My String Value");
			}
		}
	}

	//static void DebugPrintRecurs(const CRwNode& rwNode, Niflect::NifUint32 arrayItemIdx = INDEX_NONE, Niflect::NifUint32 level = 0)
	//{
	//	auto strLevel = NiflectUtil::DebugIndentToString(level);
	//	printf("%s", strLevel.c_str());
	//	if (arrayItemIdx == INDEX_NONE)
	//	{
	//		printf("%s", rwNode->GetName().c_str());
	//	}
	//	else
	//	{
	//		NIFLECT_ASSERT(rwNode->GetName().empty());
	//		printf("[%u]", arrayItemIdx);
	//	}
	//	if (rwNode->IsValue())
	//	{
	//		if (arrayItemIdx == INDEX_NONE)
	//			printf(", ");

	//		Niflect::CString str;
	//		auto rwValue = rwNode->GetValue();
	//		switch (rwValue->GetType())
	//		{
	//		case ERwValueType::Bool:
	//			str = rwValue->GetBool() ? "true" : "false";
	//			break;
	//		case ERwValueType::Float:
	//			str = std::to_string(rwValue->GetFloat()).c_str();
	//			break;
	//		case ERwValueType::Double:
	//			str = std::to_string(rwValue->GetDouble()).c_str();
	//			break;
	//		case ERwValueType::Int32:
	//			str = std::to_string(rwValue->GetInt32()).c_str();
	//			break;
	//		case ERwValueType::String:
	//			str = rwValue->GetString().c_str();
	//			break;
	//		default:
	//			NIFLECT_ASSERT(false);
	//			break;
	//		}
	//		printf("%s", str.c_str());
	//		printf("\n");
	//	}
	//	else if (rwNode->IsArray())
	//	{
	//		level++;
	//		printf("\n");
	//		auto rwArray = rwNode->GetArray();
	//		Niflect::NifUint32 idxInc = 0;
	//		for (Niflect::NifUint32 idx = 0; idx < rwArray->GetItemsCount(); ++idx)
	//			DebugPrintRecurs(rwArray->GetItem(idx), idxInc++, level);
	//	}
	//	else
	//	{
	//		level++;
	//		printf("\n");
	//		for (Niflect::NifUint32 idx = 0; idx < rwNode->GetNodesCount(); ++idx)
	//			DebugPrintRecurs(rwNode->GetNode(idx), INDEX_NONE, level);
	//	}
	//}
	static void DebugPrintRecurs2(const CRwNode* rwNode, Niflect::NifUint32 arrayItemIdx = Niflect::NifInvalidIndex, Niflect::NifUint32 level = 0)
	{
		auto strLevel = NiflectUtil::DebugIndentToString(level);
		NiflectUtil::Printf("%s", strLevel.c_str());
		if (arrayItemIdx == Niflect::NifInvalidIndex)
		{
			NiflectUtil::Printf("%s", rwNode->GetName().c_str());
		}
		else
		{
			NIFLECT_ASSERT(rwNode->GetName().empty());
			NiflectUtil::Printf("[%u]", arrayItemIdx);
		}
		if (rwNode->IsValue())
		{
			if (arrayItemIdx == Niflect::NifInvalidIndex)
				NiflectUtil::Printf(", ");

			Niflect::CString str;
			auto rwValue = rwNode->GetValue();
			switch (rwValue->GetType())
			{
			case ERwValueType::Bool:
				str = rwValue->GetBool() ? "true" : "false";
				break;
			case ERwValueType::Int8:
				str = std::to_string(rwValue->GetInt8()).c_str();
				break;
			case ERwValueType::Int16:
				str = std::to_string(rwValue->GetInt16()).c_str();
				break;
			case ERwValueType::Int32:
				str = std::to_string(rwValue->GetInt32()).c_str();
				break;
			case ERwValueType::Int64:
				str = std::to_string(rwValue->GetInt64()).c_str();
				break;
			case ERwValueType::Uint8:
				str = std::to_string(rwValue->GetUint8()).c_str();
				break;
			case ERwValueType::Uint16:
				str = std::to_string(rwValue->GetUint16()).c_str();
				break;
			case ERwValueType::Uint32:
				str = std::to_string(rwValue->GetUint32()).c_str();
				break;
			case ERwValueType::Uint64:
				str = std::to_string(rwValue->GetUint64()).c_str();
				break;
			case ERwValueType::Float:
				str = std::to_string(rwValue->GetFloat()).c_str();
				break;
			case ERwValueType::Double:
				str = std::to_string(rwValue->GetDouble()).c_str();
				break;
			case ERwValueType::String:
				str = rwValue->GetString().c_str();
				break;
			default:
				NIFLECT_ASSERT(false);
				break;
			}
			NiflectUtil::Printf("%s", str.c_str());
			NiflectUtil::Printf("\n");
		}
		else
		{
			level++;
			NiflectUtil::Printf("\n");
			for (Niflect::NifUint32 idx = 0; idx < rwNode->GetNodesCount(); ++idx)
				DebugPrintRecurs2(rwNode->GetNode(idx), rwNode->IsArray() ? idx : Niflect::NifInvalidIndex, level);
		}
	}
	static void DebugPrintTestTree()
	{
		CRwNode root;
		//DebugBuildSingleValue(&root);
		//DebugBuildSingleArray(&root);
		DebugBuildStructure(&root);
		DebugPrintRecurs2(&root);
	}
}