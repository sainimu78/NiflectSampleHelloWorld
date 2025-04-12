#pragma once
#include "Niflect/Base/Memory.h"
#include <string>
//#include <string_view>

namespace Niflect
{
	template <typename TAllocator>
	using TSTLString = std::basic_string<char, std::char_traits<char>, TAllocator>;

	using CString = TSTLString<THeapAllocator<char> >;
}

namespace std
{
	template <>
	struct hash<Niflect::CString>
	{
		size_t operator()(const Niflect::CString& s) const noexcept
		{
			//return std::hash<std::string_view>()(std::string_view(s.data(), s.size()));
			//todo: 无好办法复用std::has<std::string>, 只能接受复制字符串, 改用c++17后可用string_view避免复制
			return std::hash<std::string>{}(std::string(s.data(), s.size()));
		}
	};
}
