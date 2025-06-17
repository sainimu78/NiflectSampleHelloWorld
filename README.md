![Build Status Linux](https://github.com/sainimu78/NiflectSampleHelloWorld/actions/workflows/Linux.yml/badge.svg)
![Build Status Windows](https://github.com/sainimu78/NiflectSampleHelloWorld/actions/workflows/Windows.yml/badge.svg)

# Niflect 示例: HelloWorld

NiflectSampleHelloWorld 是最简示例项目, 用于帮助使用者掌握 C++ 原生风格反射框架 **Niflect** 的用法.

## Niflect 的特性

### 非侵入式

- 不要求继承自某个类
  - 见[例1]()
- 零运行时开销的声明式反射宏标签, 不改变被反射类型内存布局与虚表结构. 即被反射前后 `sizeof` 值相同
  - 见[例1](), [例2](), [例9]()
- 可为第三方库类型字段生成反射元数据
  - 见[例5]()

### C++ 原生风格

- 通过一种访问器设置头文件绑定访问器与其对应类型, 以实现几乎任意类型的反射与一致性保存载入
  - 见[例5]()
- 原生风格的元数据绑定, 即 IDE 友好且类型安全. 字段绑定的元数据即为 C++ 原生代码定义的类, 而非须解析且难维护的字符串
  - 见[例18]()
- 支持通过非默认构造函数实例化. 仅要求构造函数的参数为可被反射的字段类型
  - 见[例16]()
- 被反射字段类型可为别名
  - 见[例19]()

### 反射与序列化

- 支持可实例化类型定义与其几乎任意类型字段的反射与序列化
  - 多层嵌套的任意容器模板字段, 如 `std::vector<std::map<std::string, int> >`
    - 见[例4]()
  - 任意指针模板, 任意原始指针等字段, 仅须实现自定的序列化方法
    - 见[例5](), [例6]()
- 基于可编解码为任意格式的树型结构 `RwNode` 实现保存载入实例, 格式如 Niflect 提供的 JSON 编解码
  - 见[例3]()

### 动态反射

- 构建开始时通过 NiflectGenTool 生成反射代码, 在运行时仅要求在使用前初始化反射元数据
  - 见[例1](), [例12]()
- 获取类型反射元数据的函数定义通过 NiflectGenTool 生成, 运行时获取类型反射元数据接近零开销
  - 见[例2]()
- 可跨模块使用反射元数据
  - 见[例12](), [例13]()
- 通过反射元数据
  - 实例化对象, 无需依赖类型定义所在头文件
    - 见[例7](), [动态绑定类型实例的属性编辑器示例项目](https://github.com/sainimu78/NiflectSamplePropertyEditor)
  - 实现 AoS 到 SoA 的动态映射
    - 见[保持 OOP 风格的半自动 ECS 化示例项目](https://github.com/sainimu78/NiflectSampleAoS2SoA)
  - 实现内存布局, 虚表无关的实例替换
    - 见[热替换示例项目](https://github.com/sainimu78/NiflectSampleHotSwap)

## 限制

限制出于 Niflect 的设计原则

- 不追求非必要语法完备性, 从而避免实现理论几乎不可行, 实际几乎无用处的特性
- 设计目标之一是实现序列化, 实例化等架构级一致性, 从而使应用从上至下可复用基于反射实现的基础设施

#### 仅支持 C++ 14 及以上

备注: 如有实际需求可尝试适配 C++ 11

#### 不支持多继承

不支持多继承中的基类为被反射的类型, 这是为避免支持非必要语言特性可能产生的误导

#### 仅支持可被实例化类型的反射

不支持反射模板类定义, 模板偏特化字段, 即不支持带有未实例化模板参数的用法. 这是因为虽理论上可行, 但实现上将面对模板完备性的学术黑洞, 应由与编译器同级的解决方案应对

#### 解耦序列化与格式而引入的固有开销

从实例到具体序列化格式需要经过 `RwNode` 的中间树型结构, 存在固有开销, 在此特定流程下, 因该固有开销而无法达到极致序列化效率, 但架构设计能获得

- 格式无关的通用序列化流程
- 由一份头文件定义的零冗余通信协议

## 示例集

todo: 每个示例标序号, 方便引用说明, 编写带跳转的目录

### 例1. 最简示例

#### 反射类定义

使用反射宏标签 `NIF_T` 声明被反射的示例类定义 `CHelloWorld`

```c++
//HelloWorld.h
#pragma once
#include "Niflect/Default/DefaultMacroTag.h"

NIF_T()
class CHelloWorld
{
};
```

#### 初始化与使用类型反射元数据

在使用前, 通过 `InitLoadTimeModules` 初始化反射元数据

```c++
//Main.cpp
#include "HelloWorld.h"
#include "HelloWorld_private.h"

void main()
{
	Niflect::CNiflectModuleRegistry reg;
	reg.InitLoadTimeModules();

	auto type = Niflect::StaticGetType<CHelloWorld>();
	printf("Registered type: %s\n", type->GetTypeName().c_str());
}
```

通过 `StaticGetType` 以静态反射的方式获取类型反射元数据

类型反射元数据 `Niflect::CNiflectType` 中包含常用类型信息, 如本例中通过 `GetTypeName` 获取的类型名称

### 例2. 反射字段

使用反射宏标签 `NIF_F` 声明对应字段即可, 支持的字段类型可自定义, 见[例5]()

```c++
NIF_T()
class CHelloWorld
{
public:
	NIF_F()
	float m_float = 0.0f;
	NIF_F()
	std::vector<float> m_float_array;
};
```

### 例3. `RwNode` 保存, 载入, 序列化

#### 保存

通过 `SaveInstanceToRwNode` 将 `src` 保存到树型结构 `CRwNode rw` 中

```c++
Niflect::CNiflectType* type = Niflect::StaticGetType<CHelloWorld>();
CHelloWorld src;
src.m_value = 1.23f;
CRwNode rw;
type->SaveInstanceToRwNode(&src, &rw);
```

#### 载入

通过 `LoadInstanceFromRwNode` 从 `rw` 中加载 `dst`

```c++
CHelloWorld dst;
type->LoadInstanceFromRwNode(&dst, &rw);
assert(src == dst);
```

此时可观察到 `src` 与 `dst` 数据是相同的

#### 序列化

通过 `CJsonFormat::Write` 将 `rw` 序列化为 JSON 格式的字节流

```c++
Niflect::CStringStream ss;
CJsonFormat::Write(&rw, ss);
printf("%s\n", ss.str().c_str());
```

`CJsonFormat::Write` 是 Niflect 提供的常见 `RwNode` 序列化格式之一, 实际上可基于树型结构自定义 `RwNode` 的序列化格式

### 例4. 反射嵌套模板字段

使用反射宏标签 `NIF_F` 声明对应字段即可, 支持的字段类型可自定义, 见[例5]()

```c++
NIF_T()
class CHelloWorld
{
public:
	NIF_F()
	std::map<std::string, std::vector<float> > m_name_to_floats;
};
```

### 例5. 任意自定义类型字段序列化

#### 自定义实例的保存载入

实例保存载入的实现称作访问器 (Accessor)

首先定义访问器的继承类, 即定义 `CNiflectAccessor` 的继承类, 并实现 `RwNode` 的保存载入, 以常见的数据结构三维向量 `CVector3` 的保存载入为例

```c++
//ValueTypeAccessor.h
#pragma once
#include "Niflect/NiflectAccessor.h"

class CVector3
{
public:
	CVector3()
		: m_x(0.0f)
		, m_y(0.0f)
		, m_z(0.0f)
	{
	}
	float m_x;
	float m_y;
	float m_z;
};

static void SaveCVector3ToRwNode(const CVector3& vec, CRwNode* rw)
{
	auto str = NiflectUtil::FormatString("%f %f %f", vec.m_x, vec.m_y, vec.m_z);
	SetRwValueAs<Niflect::CString>(rw->ToValue(), str);
}
static void LoadCVector3FromRwNode(const CRwNode* rw, CVector3& vec)
{
	auto str = GetRwValueAs<Niflect::CString>(rw->GetValue());
	auto vecEntryStr = NiflectUtil::Split(str, ' ');
	vec.m_x = std::stof(vecEntryStr[0].c_str());
	vec.m_y = std::stof(vecEntryStr[1].c_str());
	vec.m_z = std::stof(vecEntryStr[2].c_str());
}

class CVector3Accessor : public Niflect::CNiflectAccessor
{
protected:
	virtual bool SaveInstanceImpl(const InstanceType* base, CRwNode* rw) const override
	{
		auto& instance = *static_cast<const CVector3*>(base);
		SaveCVector3ToRwNode(instance, rw);
		return true;
	}
	virtual bool LoadInstanceImpl(InstanceType* base, const CRwNode* rw) const override
	{
		auto& instance = *static_cast<CVector3*>(base);
		ASSERT(rw->IsValue());
		LoadCVector3FromRwNode(rw, instance);
		return true;
	}
};
```

#### 绑定访问器与其对应反射类型

通过 Niflect 独创的访问器设置头文件 (Accessor Setting Header), 设置自定义访问器继承类与其对应反射类型的绑定

```c++
//AccessorSetting.h
#pragma once
#include "Niflect/NiflectAccessorSetting.h"
#include "Accessor/ValueTypeAccessor.h"

using namespace NiflectAccessorSetting;

NIFAS_A() TSetting<CVector3Accessor, CVector3>;
```

AccessorSetting.h 由 NiflectGenTool 解析并生成与设置中访问器相关的反射元数据代码

对于其它类型的绑定设置方法也是相同的, 例如 Niflect 提供的 STL 常用类型的绑定设置

```c++
template <typename T0, typename T1>
NIFAS_A() TSetting<CCompoundAccessor, std::pair<T0, T1> >;

NIFAS_A() TSetting<CSTLStringAccessor, std::string>;

template <typename TInstance>
NIFAS_A() TSetting<TSTLBitsArrayAccessor<TInstance>, std::vector<bool> >;

template <typename TInstance, typename T>
NIFAS_A() TSetting<TSTLArrayAccessor<TInstance>, std::vector<T> >;

template <typename TInstance, typename T0, typename T1>
NIFAS_A() TSetting<TSTLMapAccessor<TInstance>, std::map<T0, T1>, std::pair<T0, T1> >;
```

虽写法有一定特殊性, 但支持以模板方式表达绑定设置, 风格纯原生, IDE 友好, 头文件可复用, 从而使用者可通过标准语法表达实现几乎任意类型的访问器绑定

### 例6. 反射原始指针字段

todo: 大致内容是与前述"任意自定义类型字段序列化"类似, 只需要通过全局单例获取指针即可

### 例7. 不依赖类型定义头文件的实例化

todo: 内容未完成, 须先调整如多个构造函数时的指定方法

```c++
Niflect::CNiflectType* type = Niflect::StaticGetType<CHelloWorld>();
Niflect::TSharedPtr<void*> instance = Niflect::NiflectTypeMakeShared<void*>(type);
CRwNode rw;
type->SaveInstanceToRwNode(instance.Get(), &rw);
Niflect::CStringStream ss;
CJsonFormat::Write(&rw, ss);
printf("%s\n", ss.str().c_str());
```

### 例8. 自定义反射宏标签名称

在自定义宏标签头文件中, 如 MyMacroTag.h, 定义宏标签名称

```c++
#pragma once
#include "Niflect/NiflectMacroTag.h"

#define MY_CLASS_DECL(...) _NIFLECTGENTAG_NIFRIEND

#define MY_TYPE(...) _NIFLECTGENTAG_TYPE
#define MY_FIELD(...) _NIFLECTGENTAG_FIELD
...
```

HelloWorld 的 CMakeLists.txt 中指定对应头文件路径

```cmake
...
set(v_MacroTagHeaderFilePath ${ModuleIncludePath}/MyMacroTag.h)
...
include(${c_RootThirdPartyDirPath}/NiflectGenTool/Exe.cmake)
...
```

相应使用

```c++
#pragma once
#include "HelloWorld_gen.h"

MY_TYPE()
class CHelloWorld
{
	MY_CLASS_DECL()
private:
	MY_FIELD()
	float m_value = 0.0f;
};
```

### 例9. 反射私有字段

```c++
#pragma once
#include "HelloWorld_gen.h"

NIF_T()
class CHelloWorld
{
	NIFRIEND()
private:
	NIF_F()
	float m_value = 0.0f;
};
```

### 例10. 通过实例获取反射元数据

通过实例获取类型反射元数据是常见需求, 期望用法如下

```c++
CHelloWorld src;
CReflectiveTypeBase* base = &src;
assert(base->GetType() == Niflect::StaticGetType<CHelloWorld>());
```

为保持非侵入性, Niflect 不直接提供这种功能, 因此须由使用者决定获取方式, 下为几种常见写法参考

#### 写法1, 类型反射元数据指针通过虚函数获取

该写法含虚表开销

```c++
#pragma once
#include "HelloWorld_gen.h"

class CReflectiveTypeBase
{
public:
	virtual Niflect::CNiflectType* GetType() const = 0;
};

NIF_T()
class CHelloWorld : public CReflectiveTypeBase
{
public:
	virtual Niflect::CNiflectType* GetType() const override
	{
		return Niflect::StaticGetType<CHelloWorld>();
	}
};
```

#### 写法2, 类型反射元数据指针作成员变量

该写法无虚表开销, 但须侵入构造函数

```c++
#pragma once
#include "HelloWorld_gen.h"

class CReflectiveTypeBase
{
public:
    CReflectiveTypeBase(Niflect::CNiflectType* type)
        : m_type(type)
    {
    }
    Niflect::CNiflectType* GetType() const
    {
        return m_type;
    }
    
private:
	Niflect::CNiflectType* m_type;
};

NIF_T()
class CHelloWorld : public CReflectiveTypeBase
{
	typedef CReflectiveTypeBase inherited;
public:
	CHelloWorld()
		: inherited(Niflect::StaticGetType<CHelloWorld>())
    {
    }
};
```

#### 写法3, 宏魔法封装虚函数覆盖定义

基于写法1, 指定 `-gat` 选项, 使 NiflectGenTool 在 `NIFRIEND` 宏展开中定义表示类本身的别名, 形如 `using CThis = CHelloWorld`, 从而可通过宏魔法封装

```c++
#pragma once
#include "HelloWorld_gen.h"

class CReflectiveTypeBase
{
public:
	virtual Niflect::CNiflectType* GetType() const = 0;
};

#define REFLECTIVE_TYPE_DECL()									\
	NIFRIEND()													\
	public:														\
		virtual Niflect::CNiflectType* GetType() const override	\
		{														\
			return Niflect::StaticGetType<CThis>();				\
		}														\
	private:

NIF_T()
class CHelloWorld : public CReflectiveTypeBase
{
	REFLECTIVE_TYPE_DECL()
};
```

### 例11. Niflect 提供的简易堆内存管理

Niflect 中使用的所有堆内存都通过 `Niflect::CMemory` 管理, 用 Debug 配置构建时启用内存泄漏检测

#### 验证无静态驻留内存为例

使用 `-lmr` 选项后, 可通过 `InitLoadTimeModules` 初始化反射元数据

此功能的实现方式是, 基于模块加载期间 (Load-Time) 将反射元数据注册到静态单例, 再在执行 ``InitLoadTimeModules`  时, 将注册数据移至对应的 `Niflect::CNiflectModuleRegistry`  实例中

从而避免

- 静态单例驻留问题
- 静态单例析构顺序无保证导致内存泄漏误检测问题

以下示例表明 Niflect 无静态单例驻留

```c++
auto stats = Niflect::GetDefaultMemoryStats();
assert(stats->m_bytesRuntime > 0);
{
	Niflect::CNiflectModuleRegistry reg;
	reg.InitLoadTimeModules();
}
assert(stats->m_bytesRuntime == 0);
```

- 第一个 assert 位置, 表明存在静态注册过程
- 第二个 assert 位置, 表明在 `reg` 析构后, 无驻留内存

### 例12. 模块反射元数据自动发现

todo: 大致内容是在动态库加载 `LoadLibrary`/`dlopen` 后执行 `reg.InitLoadTimeModules()` 即可

### 例13. 跨模块使用反射元数据

使用 `-gam` 选项后, NiflectGenTool 相应生成接口导出宏, 用于导出特化的类型元数据获取函数, 生成的代码形如 

```c++
namespace Niflect
{
	template <>
	_MY_SHARED_LIB_API CNiflectType* StaticGetType<CMyClass>();
}
```

### 例14. 反射成员函数与静态函数

todo: 大致内容为 NIF_M 标记函数, 着重介绍类型安全的执行方法

### 例15. 反射全局变量与全局函数

todo: 大致内容与前例"反射成员函数与静态函数"类似, 增加对获取全局变量与函数的特殊函数说明 `CNiflectType* GeneratedGetGlobalsType()`, 全局作用域中的反射元数据被当作类的字段与函数

### 例16. 反射非默认构造函数

todo: 大致内容与例14相同, 以下为备用资料

```c++
#pragma once
#include "Niflect/Default/DefaultMacroTag.h"

NIF_T()
class CHelloWorld
{
public:
    NIF_M()
	CHelloWorld(float value)
		: m_value(value)
	{
	}
	NIF_F()
	float m_value;
};
```

在构造时须确保参数形式完全正确

```c++
Niflect::CNiflectType* type = Niflect::StaticGetType<CHelloWorld>();
Niflect::TSharedPtr<void*> instance = Niflect::NiflectTypeMakeShared<void*>(type, 1.23f);
CRwNode rw;
type->SaveInstanceToRwNode(instance.Get(), &rw);
Niflect::CStringStream ss;
CJsonFormat::Write(&rw, ss);
printf("%s\n", ss.str().c_str());
```

此方式非最佳实践, 仅用于展示框架所实现的原生性

### 例17. 遍历反射元数据

todo: 演示 for (auto& it0: modules) for (auto& it1 : types) for (auto& it2 : fields)

### 例18. 将字段绑定原生类编写表示的元数据

```c++
class CMyNata : public Niflect::CNata
{
    using CThis = CMyNata;
public:
	CThis& SetExampleValue(float a) { m_value = a; return *this; }
	float m_value = 0.0f;
};

NIF_T()
class CHelloWorld
{
public:
	NIF_F(CMyNata().SetExampleValue(1.23f))
	float m_value = 0.0f;
};
```

todo: 大致内容如上代码片段, 着重介绍 IDE 友好, 易维护, 类型安全

### (本例待定)例19. 绑定字段类型别名

todo: 大致内容为, 通过 NiflectGenTool 的一个选项即可允许字段类型为在 AccessorSetting.h 中绑定的类型的别名, 此用法非最佳实践, 原因是这样可能发展为滥用别名, 使 AccessorSetting.h 机制丧失易维护的核心特性

### 例20. 集成到构建系统

todo: 大致内容为运行时 Niflect 与反射代码生成工具 NiflectGenTool, 后者以目前提供的 cmake 中一行 `include(IntegrateNiflectGenTool.cmake)` 极简集成作示例, 基础使用时只需要指定需解析的头文件列表即可, 在构建时仅文件变化时触发反射生成
