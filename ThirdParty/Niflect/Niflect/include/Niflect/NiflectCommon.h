#pragma once
#include "Niflect/NiflectDevelopmentMacro.h"//虽已加入 PrecompileHeader, 但仍须避免有时可能将带宏标签的类型所在头文件包含在 PrecompileHeader 中导致开发宏未定义的编译错误
#include <cstddef>//NULL
#define ASSERTION_ABORT
#ifdef ASSERTION_ABORT
#include <cassert>//assert
#else
#include <cstdio>//printf, fflush
#endif

enum { INDEX_NONE = -1 };

typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef long long int64;
typedef unsigned int uint32;
typedef unsigned long long uint64;

namespace Niflect
{
    enum { INVALID_HASH = 0 };
    typedef uint64 HashInt;
}

#ifdef ASSERTION_ABORT
#ifndef NDEBUG
    #define ASSERT(b)\
        do { \
            bool cond = b;\
            if (!cond)\
            {\
                assert(false);\
            }\
        } while(0)
#else
    #define ASSERT(b) (void)0
#endif
#else
#if defined(_MSC_VER)
#define DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
#define DEBUG_BREAK() __builtin_trap()
#else
#define DEBUG_BREAK() (void)0  // 其他平台无操作
#endif

#ifndef NDEBUG
    #define ASSERT(b)\
        do { \
            bool cond = b;\
            if (!cond)\
            {\
                printf("Assertion failed: %s, %d\n", __FILE__, __LINE__);\
                fflush(stdout);\
                DEBUG_BREAK();\
            }\
        } while(0)
#else
    #define ASSERT(b) (void)0
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

//#ifdef WIN32
//#define NIFLECTMODULEREG_API extern "C" __declspec(dllexport)
//#else
//#define NIFLECTMODULEREG_API extern "C" __attribute__((visibility("default")))
//#endif

//#ifdef WIN32
//#define NIFLECTTYPEREG_API __declspec(dllexport)
//#else
//#define NIFLECTTYPEREG_API __attribute__((visibility("default")))
//#endif