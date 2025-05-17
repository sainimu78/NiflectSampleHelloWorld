#pragma once
#include "Niflect/Base/String.h"
#include "Niflect/Base/Array.h"
#include <stdarg.h>
#include <fstream>
#include <algorithm>
#include <cctype>

namespace NiflectUtil
{
	//static Niflect::CString FormatString(const char* format, ...)
	//{
	//	char szContent[10240] = "";
	//	va_list va_alist;
	//	va_start(va_alist, format);
	//	vsnprintf(szContent, 10240, format, va_alist);
	//	va_end(va_alist);
	//	return szContent;
	//}
	//static std::string FormatStdString(const char* format, ...)
	//{
	//	char szContent[10240] = "";
	//	va_list va_alist;
	//	va_start(va_alist, format);
	//	vsnprintf(szContent, 10240, format, va_alist);
	//	va_end(va_alist);
	//	return szContent;
	//}

	static Niflect::CString FormatString(const char* format, ...)
	{
		va_list args;

		// 第一次获取格式化长度
		va_start(args, format);
		const int length = std::vsnprintf(nullptr, 0, format, args);
		va_end(args);

		if (length <= 0) return Niflect::CString();  // 处理无效格式或空输出

		// 准备足够大小的缓冲区
		Niflect::CString result;
		result.resize(static_cast<size_t>(length));

		// 实际执行格式化
		va_start(args, format);
		std::vsnprintf(&result[0], result.size() + 1, format, args); // +1给终止符留空间
		va_end(args);

		return result;
	}
	static float StringToFloat(const Niflect::CString& str)
	{
        return static_cast<float>(atof(str.c_str()));
	}
	static bool StartsWith(const Niflect::CString& fullString, const Niflect::CString& start)
	{
		if (fullString.length() >= start.length())
		{
			return (0 == fullString.compare(0, start.length(), start));
		}
		return false;
	}
	static bool EndsWith(const Niflect::CString& fullString, const char& ending)
	{
		if (fullString.length() >= 1)
			return fullString.back() == ending;
		return false;
	}
	static bool EndsWith(const Niflect::CString& fullString, const Niflect::CString& ending)
	{
		if(fullString.length() >= ending.length())
			return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
		return false;
	}
	static Niflect::CString ReplaceFilePathExt(const Niflect::CString& filePath, const Niflect::CString& a, const Niflect::CString& b)
	{
		auto pos = filePath.find_last_of(a);
		if (pos != std::string::npos)
		{
			auto result = filePath.substr(0, pos - (a.length() - 1));
			result += b;
			return result;
		}
		return filePath;
	}
	static Niflect::CString RemoveFileExt(const Niflect::CString& filePath, const Niflect::CString& ext = ".")
	{
		auto pos = filePath.find_last_of(ext);
		if (pos != std::string::npos)
		{
			auto result = filePath.substr(0, pos);
			return result;
		}
		return filePath;
	}
	static bool GetFileExtSafe(const Niflect::CString& filePath, Niflect::CString& extracted, const Niflect::CString& ext = ".")
	{
		auto pos = filePath.find_last_of(ext);
		if (pos != std::string::npos && pos != filePath.length() - 1)
		{
			extracted = filePath.substr(pos);
			return true;
		}
		return false;
	}
	static Niflect::CString GetFileExt(const Niflect::CString& filePath, const Niflect::CString& ext = ".")
	{
		Niflect::CString extracted;
		GetFileExtSafe(filePath, extracted, ext);
		return extracted;
	}
	static Niflect::CString ConvertToSearchPath(const Niflect::CString& path)
	{
		if (path.back() != '/')
			return path + '/';
		return path;
	}
	static Niflect::CString ConcatPath(const Niflect::CString& a, const Niflect::CString& b)
	{
		constexpr const int JoinABWithSlash = 0;
		constexpr const int ATrailingSlash = 1;
		constexpr const int BLeadingSlash = 2;
		constexpr const int BothHaveSlash = 3;
		constexpr const int BothEmpty = 4;

		auto mode = JoinABWithSlash;
		if (!a.empty() && !b.empty())
		{
			if (a.back() == '/')
				mode = ATrailingSlash;
			if (b[0] == '/')
			{
				if (mode != ATrailingSlash)
					mode = BLeadingSlash;
				else
					mode = BothHaveSlash;
			}
		}
		else if (a.empty() && !b.empty())
		{
			return b;
		}
		else if (!a.empty() && b.empty())
		{
			return a;
		}
		else
		{
			mode = BothEmpty;
		}

		Niflect::CString result;
		switch (mode)
		{
		case JoinABWithSlash:
		{
			result = a + '/' + b;
			break;
		}
		case ATrailingSlash:
		case BLeadingSlash:
		{
			result = a + b;
			break;
		}
		case BothHaveSlash:
		{
			result = b.substr(1, b.length() - 1);
			result = a + result;
			break;
		}
		default:
			break;
		}
		return result;
	}
	static bool GetParentDirPathSafe(const Niflect::CString& fileOrDirPath, Niflect::CString& parentDirPath)
	{
		auto posSlash = fileOrDirPath.find_last_of('/');
		if (posSlash != std::string::npos)
		{
			parentDirPath = fileOrDirPath.substr(0, posSlash);
			return true;
		}
		return false;
	}
	static Niflect::CString GetParentDirPath(const Niflect::CString& fileOrDirPath)
	{
		Niflect::CString parentDirPath;
		if (GetParentDirPathSafe(fileOrDirPath, parentDirPath))
			return parentDirPath;
		return parentDirPath;
	}
	static Niflect::CString GetFileName(const Niflect::CString& filePath)
	{
		auto posSlash = filePath.find_last_of('/');
		if (posSlash != std::string::npos)
			return filePath.substr(posSlash + 1, filePath.length() - posSlash);
		return filePath;
	}
	static Niflect::CString GetDirName(const Niflect::CString& dirPath)
	{
		return GetFileName(dirPath);
	}
	static void WriteStringToFile(const Niflect::CString& data, const Niflect::CString& filePath)
	{
		std::ofstream ofs(filePath.c_str(), std::ios::binary);
		if (!ofs.is_open())
		{
			printf("%s\n", filePath.c_str());
			NIFLECT_ASSERT(false);
		}
		ofs << data;
		ofs.close();
	}
	static Niflect::CString ReadStringFromFile(const Niflect::CString& filePath)
	{
		std::ifstream ifs(filePath.c_str(), std::ios::binary);
		char buffer[1024];
		Niflect::CString data;
		while (ifs.read(buffer, sizeof(buffer)))
			data.append(buffer, ifs.gcount());
		data.append(buffer, ifs.gcount());
		return data;
	}
	static Niflect::CString ConvertToUpperCase(Niflect::CString str)
	{
		std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::toupper(c); });
		return str;
	}
	static bool CompareLessHumanReadable(const Niflect::CString& a, const Niflect::CString& b)
	{
		Niflect::CString lowerA = a;
		Niflect::CString lowerB = b;
		std::transform(lowerA.begin(), lowerA.end(), lowerA.begin(), ::tolower);
		std::transform(lowerB.begin(), lowerB.end(), lowerB.begin(), ::tolower);
		return lowerA < lowerB;
	}
	static Niflect::TArray<Niflect::CString> Split(const Niflect::CString& str, char delimiter) {
		Niflect::TArray<Niflect::CString> tokens;  // 用于存储分割后的子字符串
		Niflect::CString token;                // 临时存储每个子字符串
		for (char ch : str) {             // 遍历输入字符串中的每个字符
			if (ch == delimiter) {        // 如果当前字符是分隔符
				if (!token.empty()) {     // 并且临时子字符串不为空
					tokens.push_back(token); // 将临时子字符串添加到结果向量中
					token.clear();        // 清空临时子字符串
				}
			}
			else {
				token += ch;              // 如果当前字符不是分隔符，则将其添加到临时子字符串中
			}
		}
		if (!token.empty()) {             // 如果最后一个临时子字符串不为空
			tokens.push_back(token);      // 将其添加到结果向量中
		}
		return tokens;                    // 返回分割后的子字符串向量
	}
	static Niflect::TArray<Niflect::CString> Split(const Niflect::CString& str, const Niflect::CString& delimiter) {
		Niflect::TArray<Niflect::CString> tokens;  // 用于存储分割后的子字符串
		size_t start = 0;                 // 开始位置
		size_t end = 0;                   // 结束位置

		while ((end = str.find(delimiter, start)) != std::string::npos) {
			// 找到分隔符位置
			tokens.push_back(str.substr(start, end - start)); // 提取子字符串并添加到结果向量中
			start = end + delimiter.length();                 // 更新开始位置
		}
		tokens.push_back(str.substr(start)); // 添加最后一个子字符串

		return tokens;                       // 返回分割后的子字符串向量
	}
	static Niflect::TArray<Niflect::CString> SplitPath(const Niflect::CString& path)
	{
		return NiflectUtil::Split(path, '/');
	}
	static Niflect::CString CombineFromStrings(const Niflect::TArray<Niflect::CString>& vecPath, const Niflect::CString& delim)
	{
		Niflect::CString str;
		for (Niflect::NifUint32 idx = 0; idx < vecPath.size(); ++idx)
		{
			str += vecPath[idx];
			if (idx != vecPath.size() - 1)
				str += delim;
		}
		return str;
	}
	static Niflect::CString CombineFromStrings(const Niflect::TArray<Niflect::CString>& vecPath, char delim)
	{
		Niflect::CString str(1, delim);
		return CombineFromStrings(vecPath, str);
	}
	static Niflect::CString Trim(const Niflect::CString& str)
	{
		auto is_space = [](unsigned char c) { return std::isspace(c); };

		// 找到第一个非空白字符的位置
		auto start = std::find_if_not(str.begin(), str.end(), is_space);
		// 找到最后一个非空白字符的位置
		auto end = std::find_if_not(str.rbegin(), str.rend(), is_space).base();

		// 返回修剪后的字符串
		return (start < end) ? Niflect::CString(start, end) : Niflect::CString();
	}
	static Niflect::NifUint64 StableHash(const Niflect::CString& str) {
		constexpr Niflect::NifUint64 FNV_OFFSET_BASIS = 0xcbf29ce484222325;
		constexpr Niflect::NifUint64 FNV_PRIME = 0x100000001b3;
		Niflect::NifUint64 hash = FNV_OFFSET_BASIS;
		for (char c : str) {
			hash ^= static_cast<Niflect::NifUint64>(static_cast<Niflect::NifUint8>(c));
			hash *= FNV_PRIME;
		}
		return hash;
	}
}