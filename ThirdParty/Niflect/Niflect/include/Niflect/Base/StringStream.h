#pragma once
#include "Niflect/Base/Memory.h"
#include <sstream>

namespace Niflect
{
	//begin, 旧的实现, 可兼容std::string, 使用Reflector::CMemoryStats测试, 发现无法复用基类的一些operator=导致多余的释放, 暂时保留备忘, 计划删除
	//template <typename TAllocator>
	//class TBasicString : public std::basic_string<char, std::char_traits<char>, TAllocator>
	//{
	//	typedef std::basic_string<char, std::char_traits<char>, TAllocator> inherited;
	//public:
	//	TBasicString()
	//	{
	//	}
	//	//支持执行基类函数返回基类类型, 以及支持不同Allocator
	//	template <typename TOtherAllocator>
	//	TBasicString(const std::basic_string<char, std::char_traits<char>, TOtherAllocator>& rhs)
	//		: inherited(rhs)
	//	{
	//	}
	//	TBasicString(const char* psz)
	//		: inherited(psz)
	//	{
	//	}
	//	TBasicString(const std::string& str)
	//		: inherited(str.c_str())
	//	{
	//	}
	//	//为替换为std::string后能使用std::string配套的接口, 避免c_str()能编译通过可能导致微小冗余, 因此需要明确使用处是否应该使用为c_str
	//	const char* GetRaw() const
	//	{
	//		return this->c_str();
	//	}
	//};
	//end

	template <typename TChar, typename TAllocator>
	using TSTLStringStream = std::basic_stringstream<TChar, std::char_traits<TChar>, TAllocator>;

	using CStringStream = TSTLStringStream<char, THeapAllocator<char> >;

}