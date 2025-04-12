#pragma once
#include "Niflect/Util/ConcatSymbols.h"

//General Accessor 设置, 简称 A 型设置
//可被绑定由此设置指定 Accessor 的类型有
//1. 容器模板(Container Template 1D), 如 std::vector, std::map
//2. 结构模板(Structural Template nD, n>=2), 如 std::pair
//3. 特化模板, 如 std::vector<bool>
//4. 所有能被其它设置所绑定的类型. 因此, A 型设置也可理解为一种覆盖设置
//此设置在模块范围内可多次使用, 以指定不同的类型的 Accessor 或覆盖其它设置指定的 Accessor
//TSetting 模板须指定的模板参数个数为2或3
#define NIFAS_A() using CONCAT_SYMBOLS_3(__NiflectGen_AccessorSetting_A,_,__LINE__) = 

//Compound Accessor 设置, 简称 B 型设置
//class 或 struct 定义的类型, 且带标记 NIF_T, 可绑定此设置指定的 Accessor
//当未指定类型的 A 型设置时, 则类型绑定此设置指定的 Accessor
//此设置在模块范围内只可使用1次
//TSetting 模板须指定的模板参数个数为1
#define NIFAS_B() using CONCAT_SYMBOLS_3(__NiflectGen_AccessorSetting_B,_,__LINE__) = 

//Enum Class Accessor 设置, 简称 C 型设置
//由 enum class (即 Scoped enum) 定义的枚举类型, 可绑定此设置指定的 Accessor
//当未指定类型的 A 型设置时, 则类型绑定此设置指定的 Accessor
//此设置在模块范围内只可使用1次
//TSetting 模板须指定的模板参数个数为1
#define NIFAS_C() using CONCAT_SYMBOLS_3(__NiflectGen_AccessorSetting_C,_,__LINE__) = 

//Enum Bits Mask Accessor 设置, 简称 D 型设置
//由 enum 定义的枚举类型, 可绑定此设置指定的 Accessor
//当未指定类型的 A 型设置时, 则类型绑定此设置指定的 Accessor
//此设置在模块范围内只可使用1次
//TSetting 模板须指定的模板参数个数为1
#define NIFAS_D() using CONCAT_SYMBOLS_3(__NiflectGen_AccessorSetting_D,_,__LINE__) = 

//Pointer Template Accessor 设置, 简称 E 型设置
//可被绑定由此设置指定 Accessor 的类型为特定用途的指针模板, 指针模板的定义形式类似于 std::shared_ptr, 但为特定用途
//此设置在模块范围内可多次使用, 以指定不同的类型的 Accessor 或覆盖其它设置指定的 Accessor
//TSetting 模板须指定的模板参数个数为2
#define NIFAS_E() using CONCAT_SYMBOLS_3(__NiflectGen_AccessorSetting_E,_,__LINE__) = 

namespace NiflectAccessorSetting
{
	using TemplaateArgPlaceholder = void*;
	template <typename TAccessorType, typename TBindingType = TemplaateArgPlaceholder, typename TElementBindingType = TemplaateArgPlaceholder>
	class TSetting
	{
	};
}