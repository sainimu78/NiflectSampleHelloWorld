#pragma once
#include "Niflect/NiflectCommon.h"//虽已加入 PrecompileHeader, 但仍须避免有时可能将带宏标签的类型所在头文件包含在 PrecompileHeader 中导致开发宏未定义的编译错误

enum { INDEX_NONE = Niflect::NifInvalidIndex };
enum { INVALID_HASH = Niflect::NifInvalidHash };

using int8 = Niflect::NifInt8;
using uint8 = Niflect::NifUint8;
using int16 = Niflect::NifInt16;
using uint16 = Niflect::NifUint16;
using int32 = Niflect::NifInt32;
using uint32 = Niflect::NifUint32;
using int64 = Niflect::NifInt64;
using uint64 = Niflect::NifUint64;

#define ASSERT(c) NIFLECT_ASSERT(c)