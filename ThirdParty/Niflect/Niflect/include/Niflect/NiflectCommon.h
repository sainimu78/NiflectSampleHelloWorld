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

//为实现动态类型的实例化, 通过 alignof 由编译器确定注册类型的对齐大小
//现仅为可行性实验, 由于发现开发机 VS2022 + v140(Qt5.8) 环境下无法对抽象类使用 alignof, 暂不启用
//备注, 此环境下未定义 _MSVC_TOOLSET_VERSION, _MSC_VER 为 1942, 无标准方法区分是否为 v140
//建议充分了解各平台环境, 编译器对 alignof 支持情况, 或通用获取方法后再考虑改为正式支持
//如需相关实验, 启用宏即可. 遇提示无法实例化抽象类的错误相应避免
//#define DEVMACRO_GENERATED_TYPE_ALIGNMENT