#pragma once
#include <stddef.h>

namespace Niflect
{
	using InstanceType = void;
	using OffsetType = ptrdiff_t;

	enum { OFFSET_NONE = 0 };

	inline static const InstanceType* GetOffsetAddr(const InstanceType* base, const OffsetType& offs)
	{
		return static_cast<const char*>(base) + offs;
	}
	inline static InstanceType* GetOffsetAddr(InstanceType* base, const OffsetType& offs)
	{
		return static_cast<char*>(base) + offs;
	}

	template <typename U, typename T>
	inline static OffsetType GetMemberVariableOffset(U T::*member)
	{
		return (char*)&((T*)nullptr->*member) - (char*)nullptr;
	}

	template <typename U, typename T>
	inline static OffsetType GetFieldOffset(U T::* member)
	{
		return (char*)&((T*)nullptr->*member) - (char*)nullptr;
	}
}