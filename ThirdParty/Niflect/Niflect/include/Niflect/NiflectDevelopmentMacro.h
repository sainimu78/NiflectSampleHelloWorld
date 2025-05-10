#pragma once

//调整结构, 注意到原结构为 Field 对应的 Accessor 指定 Field 在类型中的偏移而冗余创建相同的 Accessor 实例, 实际上 Accessor 与 Type 是一一对应的
#define REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING

#ifdef REFACTORING_0_TYPE_ACCESSOR_FIELD_RESTRUACTURING
	//临时屏蔽代码
	#define TEMP_DISABLED_FOR_REFACTORING_0
#endif

//改用 std::array 作 arg 数组
#define USING_STD_ARRAY_TO_FILL_ARGS