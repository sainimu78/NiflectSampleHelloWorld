#pragma once
#include "Niflect/NiflectDevelopmentMacro.h"//虽已加入 PrecompileHeader, 但仍须避免有时可能将带宏标签的类型所在头文件包含在 PrecompileHeader 中导致开发宏未定义的编译错误
#include <cstddef>//NULL
#define NIFLECTINTERNAL_ASSERTION_ABORT
#ifdef NIFLECTINTERNAL_ASSERTION_ABORT
#include <cassert>//assert
#else
#include <cstdio>//printf, fflush
#endif

namespace Niflect
{
    enum { NifInvalidIndex = -1 };
    enum { NifInvalidHash = 0 };

    typedef char NifInt8;
    typedef unsigned char NifUint8;
    typedef short NifInt16;
    typedef unsigned short NifUint16;
    typedef int NifInt32;
    typedef unsigned int NifUint32;
    typedef long long NifInt64;
    typedef unsigned long long NifUint64;

    typedef NifUint64 HashInt;
}

#ifdef NIFLECTINTERNAL_ASSERTION_ABORT
#ifndef NDEBUG
    #define NIFLECT_ASSERT(b)\
        do { \
            bool cond = b;\
            if (!cond)\
            {\
                assert(false);\
            }\
        } while(0)
#else
    #define NIFLECT_ASSERT(b) (void)0
#endif
#else
#if defined(_MSC_VER)
#define NIFLECTINTERNAL_DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
#define NIFLECTINTERNAL_DEBUG_BREAK() __builtin_trap()
#else
#define NIFLECTINTERNAL_DEBUG_BREAK() (void)0  // 其他平台无操作
#endif

#ifndef NDEBUG
    #define NIFLECT_ASSERT(b)\
        do { \
            bool cond = b;\
            if (!cond)\
            {\
                printf("Assertion failed: %s, %d\n", __FILE__, __LINE__);\
                fflush(stdout);\
                NIFLECTINTERNAL_DEBUG_BREAK();\
            }\
        } while(0)
#else
    #define NIFLECT_ASSERT(b) (void)0
#endif
#endif

#ifdef NIFLECT_API
#else
    #ifdef WIN32
        #ifdef NIFLECT_EXPORTS
            #define NIFLECT_API __declspec(dllexport)
        #else
            #define NIFLECT_API __declspec(dllimport)
        #endif
    #else
        #ifdef NIFLECT_EXPORTS
		    #define NIFLECT_API __attribute__((visibility("default")))
	    #else
		    #define NIFLECT_API 
	    #endif
    #endif
#endif