#pragma once
#include "Niflect/NiflectBase.h"
//#include "Niflect/Memory/Stats/NiflectMemoryStats.h"

namespace NiflectUtil
{
	static Niflect::CString DebugIndentToString(uint32 level, const char* singleIndentInString = "-")
	{
		auto curLevel = level;
		Niflect::CString strLevel;
		while (curLevel--)
			strLevel += singleIndentInString;
		return strLevel;
	}

	NIFLECT_API void Printf(const char* format, ...);
}