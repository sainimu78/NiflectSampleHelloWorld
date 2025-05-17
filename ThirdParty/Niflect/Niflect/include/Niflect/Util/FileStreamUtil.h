#pragma once
#include <fstream>
#include "Niflect/NiflectBase.h"

namespace NiflectUtil
{
	template <typename TStdFStream>
	class TFileStream : public TStdFStream
	{
		typedef TStdFStream inherited;
	public:
		TFileStream(const char* filePath)
			: inherited(filePath, std::ios::binary)
		{
			NIFLECT_ASSERT(this->is_open());
		}
		TFileStream(const Niflect::CString& filePath)
			: TFileStream(filePath.c_str())
		{
		}
	};

	using CInputFileStream = TFileStream<std::ifstream>;
	using COutputFileStream = TFileStream<std::ofstream>;

	//class CInputFileStream : public std::ifstream
	//{
	//	typedef std::ifstream inherited;
	//public:
	//	CInputFileStream(const char* filePath)
	//		: inherited(filePath, std::ios::binary)
	//	{
	//	}
	//	CInputFileStream(const Niflect::CString& filePath)
	//		: CInputFileStream(filePath.c_str())
	//	{
	//	}
	//};

	template <typename TFStream>
	static bool OpenFileStream(TFStream& stm, const char* filePath)
	{
		stm.open(filePath, std::ios::binary);
		return stm.is_open();
	}
	template <typename TFStream>
	static bool OpenFileStream(TFStream& stm, const Niflect::CString& filePath)
	{
		return OpenFileStream<TFStream>(stm, filePath.c_str());
	}
	//static std::ifstream ifstream(const char* filePath)
	//{
	//	std::ifstream ifs(filePath, std::ios::binary);
	//	return ifs;
	//}
	//static std::ofstream ofstream(const char* filePath)
	//{
	//	std::ofstream ofs(filePath, std::ios::binary);
	//	return ofs;
	//}
	//template <typename TStr>
	//static std::ifstream ifstream(const TStr& filePath)
	//{
	//	std::ifstream ifs(filePath.c_str(), std::ios::binary);
	//	return ifs;
	//}
	//template <typename TStr>
	//static std::ofstream ofstream(const TStr& filePath)
	//{
	//	std::ofstream ofs(filePath.c_str(), std::ios::binary);
	//	return ofs;
	//}
}