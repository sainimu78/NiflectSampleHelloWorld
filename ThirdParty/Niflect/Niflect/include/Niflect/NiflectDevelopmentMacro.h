#pragma once

//调整结构, 注意到原结构为 Field 对应的 Accessor 指定 Field 在类型中的偏移而冗余创建相同的 Accessor 实例, 实际上 Accessor 与 Type 是一一对应的
#define REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING

#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
	//临时屏蔽代码
	#define TEMP_DISABLED_FOR_REFACTORING_0
#endif

//改用 std::array 作 arg 数组
#define USING_STD_ARRAY_TO_FILL_ARGS

//为实现动态类型的实例化, 通过 alignof 由编译器确定注册类型的对齐大小
//现仅为可行性实验, 由于发现开发机 VS2022 + v140(Qt5.8) 环境下无法对抽象类使用 alignof, 暂不启用
//备注, 此环境下未定义 _MSVC_TOOLSET_VERSION, _MSC_VER 为 1942, 无标准方法区分是否为 v140
//建议充分了解各平台环境, 编译器对 alignof 支持情况, 或通用获取方法后再考虑改为正式支持
//如需相关实验, 启用宏即可. 遇提示无法实例化抽象类的错误相应避免
//#define NIFLECTDEV_GENERATED_TYPE_ALIGNMENT