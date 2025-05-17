#pragma once
#include "Niflect/NiflectBase.h"
#include "Niflect/NiflectCommon.h"

namespace NiflectUtil
{
	NIFLECT_API void MakeDirectories(const Niflect::CString& file_path);
	NIFLECT_API bool DeleteDirectory(const Niflect::CString& dir);
	NIFLECT_API bool DirectoryExists(const Niflect::CString& dir);
	NIFLECT_API bool FileExists(const Niflect::CString& path);
	NIFLECT_API Niflect::CString GetCurrentWorkingDirPath();
	NIFLECT_API bool SetCurrentWorkingDirPath(const Niflect::CString& path);
	NIFLECT_API bool IsRelativePath(const Niflect::CString& path);
	NIFLECT_API Niflect::CString ResolvePath(const Niflect::CString& relativePath);

	struct SCollectingContext
	{
		const Niflect::CString& m_dirPath;
		const Niflect::CString& m_fileName;
	};
	typedef std::function<void(const SCollectingContext& ctx)> OnCollectedFunc;

	class CCollectingOption
	{
		typedef CCollectingOption CThis;
	public:
		CCollectingOption(const OnCollectedFunc& Func)
			: m_maxDepth(0)
			, m_Func(Func)
		{
		}
		CThis& SetMaxDepth(Niflect::NifUint32 depth)
		{
			m_maxDepth = depth;
			return *this;
		}
		Niflect::NifUint32 m_maxDepth;
		OnCollectedFunc m_Func;
	};

	NIFLECT_API void CollectFiles(const Niflect::CString& dirPath, const CCollectingOption& opt);

	NIFLECT_API bool CmdExec(const Niflect::CString& cmd, Niflect::CString* result = NULL);
	NIFLECT_API bool CmdExec(const Niflect::CString& cmd, Niflect::CString& result);
	NIFLECT_API bool CmdExecNonblocking(const Niflect::CString& cmd);
}