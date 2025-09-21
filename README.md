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
- 不要求启用 RTTI, 反射元数据无虚表开销

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
  - 特化模板字段, 如 `std::vector<bool>`
    - 见[例4]()
  - 任意指针模板, 任意原始指针等字段, 仅须实现自定的序列化方法
    - 见[例5](), [例6]()
- 可基于反射元数据实现创建实例, 不要求实例使用 Niflect 提供的内存管理
  - 见[例24]()
- 可根据必要的反射元数据遍历与处理, 实现通用或自定义的实例序列化流程, 例如实现接入主流的序列化第三方库或实现完全自定义的序列化框架
  - 见[例21](), [例22](), [例23]()
- 可选组件: 平衡通用性与性能的序列化框架 `RwTree`
  - 基于可编解码为任意格式的通用树型结构 `RwNode` 实现保存载入实例, 可序列化的格式如 Niflect 提供的 JSON 格式, 见[例3]()
  - 从实例到具体序列化格式需要经过 `RwNode` 的中间树型结构, 存在固有开销, 在此特定流程下, 因该固有开销而无法达到极致序列化效率, 但架构设计能获得难以拒绝的通用性与扩展性

    - 格式无关的通用序列化流程
    - 由一份头文件定义的零冗余通信协议

### 动态反射

- 构建开始时通过 NiflectGenTool 生成反射代码, 在运行时仅要求在使用前初始化反射元数据
  - 见[例1](), [例12]()
- 获取类型反射元数据的函数定义通过 NiflectGenTool 生成, 运行时获取类型反射元数据接近零开销
  - 见[例1]()
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

## 示例集

todo: 每个示例标序号, 方便引用说明, 编写带跳转的目录

### 例1. 最简示例

#### 反射类定义

使用反射宏标签 `NIF_T` 声明被反射的示例类定义 `CHelloWorld`

```c++
//HelloWorld.h
#pragma once
#include "Niflect/Component/DefaultMacroTag.h"

NIF_T()
class CHelloWorld
{
};
```

#### 初始化与获取类型反射元数据

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

通过 `StaticGetType` 以静态反射的方式获取类型反射元数据, 该函数由 NiflectGenTool 生成, 定义形如

```c++
//_HelloWorld_gen_private.h
namespace Niflect
{
	template <>
	CNiflectType* StaticGetType<CHelloWorld>()
	{
		return TRegisteredType<CHelloWorld>::s_type;
	}
}
```

其中 `s_type` 的定义为类型反射元数据指针

```c++
//NiflectRegisteredType.h
namespace Niflect
{
	template <typename T>
	class TRegisteredType
	{
        ...
		static CNiflectType* s_type;
	};
}
```

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
SaveInstanceToRwNode(type, &src, &rw);
```

#### 载入

通过 `LoadInstanceFromRwNode` 从 `rw` 中加载 `dst`

```c++
CHelloWorld dst;
LoadInstanceFromRwNode(type, &dst, &rw);
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

### 例4. 反射嵌套容器模板字段, 特化模板字段

使用反射宏标签 `NIF_F` 声明对应字段即可, 支持的字段类型可自定义, 见[例5]()

```c++
NIF_T()
class CHelloWorld
{
public:
	NIF_F()
	std::map<std::string, std::vector<float> > m_name_to_floats;
	NIF_F()
	std::vector<bool> m_bits_array;
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

class CVector3Accessor : public CRwAccessor
{
protected:
	virtual bool SaveImpl(const InstanceType* base, CRwNode* rw) const override
	{
		auto& instance = *static_cast<const CVector3*>(base);
		SaveCVector3ToRwNode(instance, rw);
		return true;
	}
	virtual bool LoadImpl(InstanceType* base, const CRwNode* rw) const override
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
//StandardRwAccessorSetting.h
...
NIFAS_A() TSetting<CSTLStringRwAccessor, std::string>;

template <typename TInstance>
NIFAS_A() TSetting<TSTLBitsArrayRwAccessor<TInstance>, std::vector<bool> >;

template <typename TInstance, typename T>
NIFAS_A() TSetting<TSTLArrayRwAccessor<TInstance>, std::vector<T> >;

template <typename TInstance, typename T0, typename T1>
NIFAS_A() TSetting<TSTLMapRwAccessor<TInstance>, std::map<T0, T1>, std::pair<T0, T1> >;
...
```

虽写法有一定特殊性, 但支持以模板方式表达绑定设置, 风格纯原生, IDE 友好, 头文件可复用, 从而使用者可通过标准语法表达实现几乎任意类型的访问器绑定

### 例6. 反射原始指针字段

示例类 `CResource`, 表示一种可通过 ID 查找的实例, 如文件资源

#### 反射原始指针字段

设置访问器绑定

```c++
NIFAS_A() TSetting<CResourceAccessor, CResource*>;
```

使用宏标签声明

```c++
NIF_T()
class CHelloWorld
{
public:
	...
	NIF_F()
	CResource* m_res = NULL;
};
```

#### 序列化测试

```c++
static void InitForTest(CHelloWorld& instance)
{
	...
	instance.m_res = GetResourceFactory()->Find("Note_B.txt");
}
static bool operator==(const CHelloWorld& lhs, const CHelloWorld& rhs)
{
	return ...
		&& lhs.m_res == rhs.m_res
		;
}

int main()
{
	auto factory = GetResourceFactory();
	factory->Register("Note_A.txt");
	factory->Register("Note_B.txt");
	factory->Register("Note_C.txt");
	{
		Niflect::CNiflectModuleRegistry reg;
		reg.InitLoadTimeModules();
		CHelloWorld src;
		InitForTest(src);
		auto type = Niflect::StaticGetType<CHelloWorld>();
		CRwNode rw;
		SaveInstanceToRwNode(type, &src, &rw);
		Niflect::CStringStream ss;
		CJsonFormat::Write(&rw, ss);
		printf("%s\n", ss.str().c_str());
	}
	DestroyResourceFactory();
	return 0;
}
```

可观察到打印结果中含初始化时指定的 `Note_B.txt`, 即实现原始指针的反射与序列化

#### 示例类 `CResource` 与 `CResourceFactory`

定义示例类 `CResource`, 表示一种可通过 ID 查找的实例类型, 如表示文件资源类

```c++
class CResource
{
public:
	const Niflect::CString* m_id = NULL;
};
using CSharedResource = Niflect::TSharedPtr<CResource>;
```

定义指针容器单例类 `CResourceFactory`, 表示 ID 与 `CResource` 的绑定查找表

```c++
class CResourceFactory
{
public:
	CResource* Register(const Niflect::CString& id)
	{
		auto ret = m_mapIdToIdx.insert({ id, static_cast<int>(m_vecRes.size())});
		const auto& idx = ret.first->second;
		if (ret.second)
		{
			auto res = Niflect::MakeShared<CResource>();
			res->m_id = &ret.first->first;
			m_vecRes.push_back(res);
		}
		return m_vecRes[idx].Get();
	}
	CResource* Find(const Niflect::CString& id) const
	{
		return m_vecRes[m_mapIdToIdx.at(id)].Get();
	}

private:
	Niflect::TMap<Niflect::CString, int> m_mapIdToIdx;
	Niflect::TArray<CSharedResource> m_vecRes;
};
using CSharedResourceFactory = Niflect::TSharedPtr<CResourceFactory>;

static CSharedResourceFactory* s_addr = NULL;
CResourceFactory* GetResourceFactory()
{
	static auto s_singleton = Niflect::MakeShared<CResourceFactory>();
	if (s_addr == NULL)
		s_addr = &s_singleton;
	return s_singleton.Get();
}
void DestroyResourceFactory()
{
	*s_addr = NULL;
	s_addr = NULL;
}
```

#### 示例访问器 `CResourceAccessor`

使用 `GetResourceFactory` 获取单例, 实现原始指针的保存载入

```c++
class CResourceAccessor : public CRwAccessor
{
    using PointerType = CResource*;
protected:
    virtual bool SaveImpl(const InstanceType* base, CRwNode* rw) const
    {
        auto& instance = *static_cast<const PointerType*>(base);
        SetRwValueAs<Niflect::CString>(rw->ToValue(), *instance->m_id);
        return true;
    }
    virtual bool LoadImpl(InstanceType* base, const CRwNode* rw) const
    {
        auto& instance = *static_cast<PointerType*>(base);
        instance = GetResourceFactory()->Find(GetRwValueAs<Niflect::CString>(rw->GetValue()));
        return true;
    }
};
```

#### 总结

本例体现以下特性

##### 非侵入性

不要求 `CResource` 含任何标记或特殊定义

##### 任意序列化

示例中的 `GetResourceFactory()->Find` 可任意替换为其它实现形式, 例如可引入某种索引化方法消除查找开销, 或引入某种状态标志避免流程冲突等等

##### 复用性

可将 `CResource` 相关类型改为模板或其它方式的实现, 以复用一些特定用途指针的序列化方法

##### 易用性

有时可能需要引入所有权管理, 反射 `std::shared_ptr` 的指针.

只需要以类似方法, 即基于单例容器实现指针序列化的方法, 定义相关类型并设置访问器绑定

```c++
NIFAS_A() TSetting<CSharedResourceAccessor, CSharedResource>;
```

或使用模板作通用化设置

```c++
template <typename TInstance, typename T>
NIFAS_E() TSetting<TSharedResourceAccessor<TInstance>, std::shared_ptr<T> >;
```

### 例7. 不依赖类型定义头文件的实例化

通过 `MakeSharedInstance` 可使用默认构造函数创建实例

```c++
#include "Niflect/Component/SharedInstance.h"

void main()
{
    ...
    auto type = Niflect::StaticGetType<CHelloWorld>();
    Niflect::TSharedPtr<void> instance = Niflect::MakeSharedInstance<void>(type);
    CRwNode rw;
    type->SaveInstanceToRwNode(instance.Get(), &rw);
    Niflect::CStringStream ss;
    CJsonFormat::Write(&rw, ss);
    printf("%s\n", ss.str().c_str());   
}
```

当然地, 对应类型必须显式或隐式定义默认构造函数, 非默认构造函数创建实例见[例16]()

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

`NIFRIEND` 宏展开代码通过 NiflectGenTool 生成, 默认时该宏反射代码仅含 `friend` 声明, 形如

```c++
friend class Niflect::CTypeBody;
```

作用即为反射生成的代码可访问对应类的私有定义

`NIFRIEND` 的定义具特殊性, 宏展开的最后一行为 `private:`, 这是为了保持 `class` 定义默认为私有的规则, 因此约定该宏必须在 `class` 定义中的首行指定

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

该宏还有扩展用法, 见[例10]()

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

定义动态加载模板的帮助类 `CRunTimeLinkingLibrary`

```c++
class CRunTimeLinkingLibrary
{
public:
	bool Load(const Niflect::CString& dirPath, const Niflect::CString& libName)
	{
		const auto filePath = ConvertToLibFilePath(dirPath, libName);
#if defined(_WIN32)
		m_handle = LoadLibrary(filePath.c_str());
#else
		m_handle = dlopen(filePath.c_str(), RTLD_LAZY);
#endif
		return true;
	}

private:
	void* m_handle = NULL;
};
```

只需要在 `CRunTimeLinkingLibrary::Load` 之后调用 `InitLoadTimeModules` 即可

```c++
class CRunTimeModule
{
public:
	bool Reload(const Niflect::CString& dirPath, const Niflect::CString& libName)
	{
        m_reg.Clear();
		if (m_lib.Load(dirPath, libName))
			return m_reg.InitLoadTimeModules();
		return false;
	}

private:
	CRunTimeLinkingLibrary m_lib;
	Niflect::CNiflectModuleRegistry m_reg;
};
```

这与[例1]()所示, 对静态初始化阶段加载的模块初始化反射元数据方法是一致的, 区别仅在于本例的模块为动态加载

### 例13. 跨模块使用反射元数据

使用 `-gam` 选项后, NiflectGenTool 相应生成接口导出宏, 用于导出特化的类型元数据获取函数

以名为 ExampleModule 的示例 Shared Library 为例, 在模块中定义的类型

```c++
NIF_T()
class CExampleType
{
};
```

对应生成的代码为 

```c++
namespace Niflect
{
	template <>
	_EXAMPLE_MODULE_API CNiflectType* StaticGetType<CExampleType>();
}
```

在静态初始化阶段 (Load-Time) 链接 ExampleModule 的模块代码中执行 `InitLoadTimeModules` 一并初始化 ExampleModule 中的反射元数据. 从而通过 `StaticGetType` 即可获取导出的类型反射原数据

```c++
#include "ExampleType.h"

void main()
{
	Niflect::CNiflectModuleRegistry reg;
	reg.InitLoadTimeModules();
	Niflect::CNiflectType* type = Niflect::StaticGetType<CExampleType>();
	printf("%s\n", type->GetTypeName().c_str());
}
```

### 例14. 反射成员函数与静态函数

使用 `UnsafeInvokeMethod` 即可通过函数反射元数据调用函数, 此为非参数类型安全的调用方式, 与参数类型安全的调用方式 SETI 相比, 已无明显优势, 因此不作详细说明

#### SETI - 单参数类型擦除调用

SETI 表示单参数类型擦除调用 (Single-Argument Erasure for Type-Safe Invocation), 是一种使用反射元数据调用函数的最佳实践方法, 具备传统方法无法同时实现的特性

- 调用开销接近直接函数调用
- 参数类型擦除
- 参数类型运行时安全检查
- 虚表无关

本方法的思想是将函数的参数理解为类定义的字段

因此 SETI 仅要求函数参数定义符合其中一项

- 无参数
- 将所有参数改写为某个反射类的字段

#### 反射成员函数

定义函数的 SETI 参数类, 使用 `NIF_T` 声明以支持类型安全的检查

```c++
NIF_T()
class CInvocationContext
{
public:
    float m_in_arg0 = 0.0f;
    int m_in_arg1 = 0;
    bool m_out = false;
};
```

使用 `NIF_M` 声明反射的函数

```c++
NIF_T()
class CHelloWorld
{
public:
    NIF_M()
    void FuncA(CInvocationContext& ctx)
    {
        printf("%f, %d\n", ctx.m_in_arg0, ctx.m_in_arg1);
        ctx.m_out = true;
    }
};
```

#### 查找函数反射元数据索引

查找方法可为任意, 通常以如下方式查找

- 通过 Nata 绑定虚表无关的函数签名
- 通过 Nata 绑定枚举或某种标识
- 通过函数名

本例以最易理解的函数名演示查找方法

```c++
static NifUint32 FindMethodIndex(const Niflect::TArray<Niflect::CMethodInfo>& vec)
{
    NifUint32 foundIdx = NifInvalidIndex;
    for (NifUint32 idx = 0; idx < vec.size(); ++idx)
    {
        auto& it = vec[idx];
        if (it.m_name == "FuncA")
        {
            foundIdx = idx;
            break;
        }
    }
    return foundIdx;
}
```

#### SETI

自行确保参数正确, 仅 Debug 时提示相应错误的调用方式

```c++
template <typename TArg>
static void InvokeMethodSETI(Niflect::CNiflectType* type, const Niflect::CString& funcName, Niflect::InstanceType* base, TArg& arg)
{
    NifUint32 foundIdx = FindMethodIndex(funcName, type->m_vecMethodInfo);
	type->InvokeMethod(foundIdx, base, arg);
}
```

带正确性检查的调用方式, 正确则返回 `true`

```c++
template <typename TArg>
static bool InvokeMethodCheckedSETI(Niflect::CNiflectType* type, const Niflect::CString& funcName, Niflect::InstanceType* base, TArg& arg)
{
    NifUint32 foundIdx = FindMethodIndex(funcName, type->m_vecMethodInfo);
	return type->InvokeMethodChecked(foundIdx, base, arg);
}
```

### 例15. 反射全局变量与全局函数

Niflect 框架中, 全局变量与全局函数分别被理解为一种全局作用域类中的成员变量与成员函数, 因此二者的反射方法与对类的成员变量怀成员函数是相同的, 另见[例14]()

```c++
//ExampleModule.h
#pragma once
#include "Niflect/Component/DefaultMacroTag.h"

NIF_F()
extern float g_exampleGlobalThreshold;

NIF_M()
void ExampleGlobalFunc();
```

须通过生成的函数 `GeneratedGetGlobalsType` 获取相应反射元数据

```c++
#include "ExampleModule.h"
#include "ExampleModule_private.h"

void PrintGlobalVariables()
{
    Niflect::CNiflectType* type = Niflect::GeneratedGetGlobalsType();
    for (auto& it : type->GetFields())
        printf("%s\n", it.GetName().c_str());
}
```

### 例16. 反射非默认构造函数

非默认构造函数须通过 `NIF_M` 声明, 以无默认构造函数的定义方式为例

```c++
#pragma once
#include "Niflect/Default/DefaultMacroTag.h"

NIF_T()
class CHelloWorld
{
public:
    NIF_M()
	CHelloWorld(float someMeaningfulValue)
		: m_someMeaningfulValue(someMeaningfulValue)
	{
	}
	NIF_F()
	float m_someMeaningfulValue;
};
```

在构造时须自行确保参数形式完全正确

```c++
Niflect::CNiflectType* type = Niflect::StaticGetType<CHelloWorld>();
Niflect::TSharedPtr<void> instance = Niflect::UnsafeMakeSharedInstance<void>(type, 1.23f);
CRwNode rw;
SaveInstanceToRwNode(type, instance.Get(), &rw);
Niflect::CStringStream ss;
CJsonFormat::Write(&rw, ss);
printf("%s\n", ss.str().c_str());
```

当可确保参数定义不需要扩展性时, 此方式是可使用的, 但必须明确一点, 此方式非最佳实践, 因此建议使用 SETI 方式创建实例

```c++
NIF_T()
class CCtorContext
{
public:
    float m_in_arg0 = 0.0f;
};

NIF_T()
class CHelloWorld
{
public:
    NIF_M()
	CHelloWorld(CCtorContext& ctx)
        : m_someMeaningfulValue(ctx.m_in_arg0)
	{
	}
	NIF_F()
	float m_someMeaningfulValue;
};
```

带类型安全检查的创建实例

```c++
Niflect::CNiflectType* type = Niflect::StaticGetType<CHelloWorld>();
CCtorContext ctx;
ctx.m_in_arg_0 = 1.23f;
Niflect::TSharedPtr<void> instance;
Niflect::MakeSharedInstanceChecked<void>(type, instance, ctx);
CRwNode rw;
SaveInstanceToRwNode(type, instance.Get(), &rw);
Niflect::CStringStream ss;
CJsonFormat::Write(&rw, ss);
printf("%s\n", ss.str().c_str());
```

### 例17. 遍历反射元数据

```c++
Niflect::CNiflectModuleRegistry reg;
for (auto& module : reg.GetModules())
{
    printf("Module: %s\n", module.GetName().c_str());
    for (auto& type : module.GetTable().GetTypes())
    {
        printf("- %s\n", type->GetTypeName().c_str());
        for (auto& field : type->GetFields())
    		printf("-- %s\n", field.GetName().c_str());
    }
}
```

### 例18. 将字段绑定原生类编写表示的元数据

#### Nata - 原生元数据

Nata 表示原生元数据 (Native Metadata), 用于类型, 字段, 函数宏标签绑定的元数据定义, 特点在于通过原生风格定义

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

可见定义方法为纯原生语法, 具备

- IDE 友好
- 易维护
- 类型安全
- 可封装, 可简化, 可用模板定义

这些特性已显著优于传统基于字符串的元数据定义方式

更重要的是, Nata 概念具备实现领域元数据标准化的潜力, 这是字符串元数据不可能实现的目标

### (本例待定) 例19. 绑定字段类型别名

通过 NiflectGenTool 的 `-aft` 选项, 允许字段类型为在 AccessorSetting.h 中绑定类型的别名

需要注意的是此用法非最佳实践, 因为这可能导致滥用别名, 使 AccessorSetting.h 机制丧失易维护的核心特性

### 例20. 集成到构建系统

目前 Niflect 处实验阶段, 未迁移到主流包管理工具

简单步骤集成的前提是使用 [CMakeProjectFramework](https://github.com/sainimu78/CMakeProjectFramework) 的 cmake 项目框架, 如不使用, 则按常规的 Executable 与 Shared Library 集成方法相应操作即可

#### NiflectGenTool

集成 NiflectGenTool 反射代码生成工具

通过局部作用域的变量配置工具功能, 如通过 `v_ListModuleHeaderFilePath` 指定需解析的头文件

```cmake
list(APPEND v_ListModuleHeaderFilePath ${ModuleHeaders})
include(${c_RootThirdPartyDirPath}/NiflectGenTool/Exe.cmake)
```

集成后仅相应头文件变化才会触发反射代码生成, 因此不会产生冗余构建

其它功能可参考 IntegrateNiflectGenTool.cmake

#### Niflect

集成 Niflect 运行时以使用反射元数据

```cmake
include(${c_RootThirdPartyDirPath}/Niflect/Shared.cmake)
```

### 例21. 完全自定义序列化流程

Niflect 中, 对访问器 (Accessor) 的实现方式上, 仅要求访问器继承自 `CNiflectAccessor`, 对保存载入的具体实现无要求

示例集中提到的 `RwTree` 框架以及相关类型, 是 Niflect 提供的一种序列化流程的实现, 不是必须使用的, 使用者可根据需要实现序列化流程

#### 访问器基类

要求必须继承自 `CNiflectAccessor` 且必须在保存载入过程中可获取实例地址 , 如

```c++
//MyAccessorA.h
class CMyAccessorA : public CNiflectAccessor
{
public:
	virtual bool Save(const InstanceType* base, CMyOutputArchive& ar) const = 0;
	virtual bool Load(InstanceType* base, CMyInputArchive& ar) const = 0;
};
```

其中 `base` 为被保存载入的实例地址, 但并不要求必须定义在 `Save` 与 `Load` 函数参数中, 实际上只要实现能正确获取该地址即可

其中 `CMyInputArchive` 与 `CMyOutputArchive` 表示所需的某种序列化实现, 无任何要求

其中 `Save` 与 `Load` 可任意定义, 例如改为合并 `Save` 与 `Load` 的风格

```c++
class CMyAccessorB : public CNiflectAccessor
{
public:
	virtual bool Serialize(InstanceType* base, CMyArchive& ar) const = 0;
};
```

参数形式可自定义, 例如改为引入序列化过程优化用的内存管理对象

```c++
class CMyAccessorC : public CNiflectAccessor
{
public:
	virtual bool Save(const InstanceType* base, CMyOutputArchive& ar, CMyAllocator& allocator) const = 0;
	virtual bool Load(InstanceType* base, CMyInputArchive& ar) const = 0;
};
```

#### 序列化流程

为实现通用性, 流程的实现风格通常为递归方式

```c++
//MyAccessorA.h
static bool SaveInstance(CNiflectType* type, const InstanceType* base, CMyOutputArchive& ar)
{
	for (auto& it : type->GetTypeLayout())
	{
		if (!it->GetDerivedAccessor<CMyAccessor>()->Save(base, ar))
			return false;
	}
	return true;
}
static bool LoadInstance(CNiflectType* type, InstanceType* base, CMyInputArchive& ar)
{
	for (auto& it : type->GetTypeLayout())
	{
		if (!it->GetDerivedAccessor<CMyAccessor>()->Load(base, ar))
			return false;
	}
	return true;
}
```

#### 序列化 `float` 类型

以建议风格 `CMyAccessorA` 为例, 序列化 `float` 类型的实例, 相应实现 `Save` 与 `Load` 即可

```c++
//ValueTypeMyAccessorA.h
class CFloatMyAccessorA : public CMyAccessorA
{
public:
	virtual bool Save(const InstanceType* base, CMyOutputArchive& ar) const override
	{
		auto& instance = *static_cast<const float*>(base);
		ar.Encode<float>(instance);
		return true;
	}
	virtual bool Load(InstanceType* base, CMyInputArchive& ar) const override
	{
		auto& instance = *static_cast<TValue*>(base);
		ar.Decode<float>(instance);
		return true;
	}
};
```

#### 序列化类与结构体类型

结合序列化流程的 `SaveInstance` 与 `LoadInstance`, 通过遍历类型反射元数据实现保存载入类或结构体类型的实例

```c++
//InheritableTypeMyAccessorA.h
class CInheritableTypeMyAccessor : public CMyAccessor
{
public:
	virtual bool Save(const InstanceType* base, CMyOutputArchive& ar) const override
	{
		for (auto& it : this->GetOwnerType()->GetFields())
		{
			auto& nextAr = ar.Next(it.GetName());
			if (!SaveInstance(it.GetType(), it.GetAddr(base), nextAr))
				return false;
		}
		return true;
	}
	virtual bool Load(InstanceType* base, const CMyInputArchive* rw) const override
	{
		for (auto& it : this->GetOwnerType()->GetFields())
		{
			auto& nextAr = ar.Next(it.GetName());
			if (!LoadInstance(it.GetType(), it.GetAddr(base), nextAr))
				return false;
		}
		return true;
	}
};
```

#### 访问器设置头文件

```c++
//MyAccessorSetting.h
#pragma once
#include "Niflect/NiflectAccessorSetting.h"
#include "ValueTypeMyAccessor.h"
#include "InheritableTypeMyAccessorA.h"

namespace MyAccessorSetting
{
	using namespace NiflectAccessorSetting;

	NIFAS_B() TSetting<CInheritableTypeMyAccessor>;
	NIFAS_A() TSetting<CFloatMyAccessorA, float>;
}
```

其中 `NIFAS_A` 与 `NIFAS_B` 表示特定类型的绑定, 包括其它标签的具体使用说明, 见 NiflectAccessorSetting.h

以使用 `CMakeProjectFramework` 为例, 通过变量指定 NiflectGenTool 所需的访问器设置头文件路径

```cmake
list(APPEND v_ListModuleHeaderFilePath ${ModuleHeaders})
...
list(APPEND v_ListAccessorSettingHeaderFilePath ${ModuleSourcePath}/AccessorSetting.h)
...
include(${c_RootThirdPartyDirPath}/NiflectGenTool/Exe.cmake)
```

#### 序列化流程测试

本例中仅演示定义 `CFloatMyAccessorA` 与 `CInheritableTypeMyAccessor`, 因此测试用例如下

```c++
//MyClass.h
#pragma once
#include "Niflect/Component/DefaultMacroTag.h"

NIF_T()
class CMyClass
{
public:
	void Init()
    {
        m_float_0 = 1.23f;
    }
    bool operator==(const CMyClass& rhs) const
    {
        return m_float_0 == rhs.m_float_0
            ;
    }
    
public:
    NIF_F()
    float m_float_0 = 0.0f;
}
```

序列化测试

```c++
//MyAccessorExample.cpp
#include "MyClass.h"
#include "MyAccessorExample_private.h"

void main()
{
    CMyClass src;
    src.Init();
    CMyClass dst;
    CMyMemoryStream data;
    
	Niflect::CNiflectModuleRegistry reg;
	reg.InitLoadTimeModules();
	auto type = Niflect::StaticGetType<CMyClass>();
    
    CMyOutputArchive oar(data);
    SaveInstance(type, &src, oar);
        
    CMyInputArchive iar(data);
    LoadInstance(type, &dst, iar);
    
    assert(src == dst);
}
```

#### 总结

Niflect 提供的 `RwTree` 组件同样是按照此方式实现, 无特殊处理, 此方式即为 Niflect 框架中的序列化流程标准实现方法

实现序列化流程并不是项简单的工作, 但仅须深刻理解背后的一条规则即可完全掌握实现方法

- 通过访问器设置头文件设置访问器与类型的绑定, 其余实现几乎可全部自定义, 这也是 Niflect 具备原生性的重要体现

### 例22. 接入第三方序列化库

以模板定义访问器 `TMyBridgeAccessor`, 从而可接入 boost 的序列化模板函数

```c++
class CMyAccessor : public Niflect::CNiflectAccessor
{
public:
	virtual bool Save(const InstanceType* base, boost::archive::binary_oarchive& oarchiver) const { return false; }
	virtual bool Load(InstanceType* base, boost::archive::binary_iarchive& iarchiver) const { return false; }
};

template <typename TType>
class TMyBridgeAccessor : public CMyAccessor
{
public:
	virtual bool Save(const InstanceType* base, boost::archive::binary_oarchive& oarchiver) const override
	{
		auto& instance = *static_cast<const TType*>(base);
		oarchiver << instance;
		return true;
	}
	virtual bool Load(InstanceType* base, boost::archive::binary_iarchive& iarchiver) const override
	{
		auto& instance = *static_cast<TType*>(base);
		iarchiver >> instance;
		return true;
	}
};

template <typename TInstance>
NIFAS_B() TSetting<TMyBridgeAccessor<TInstance> >;
```

定义示例类

```c++
NIF_T()
class CRecord
{
private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int)
	{
		ar & m_ids;
		ar & m_strings;
	}

public:
	NIF_F()
	std::vector<int64_t> m_ids;
	NIF_F()
	std::vector<std::string> m_strings;
};
```

`class` 的类型以通过上述 `NIFAS_B` 指定以模板定义的访问器, NiflectGenTool 将为 `CRecord` 生成创建 `TMyBridgeAccessor<CRecord>` 访问器的代码

使用 boost 的二进制格式读写

```c++
auto type = Niflect::StaticGetType<CRecord>();
std::stringstream ss;
boost::archive::binary_oarchive oarchiver(ss);
SaveInstanceToBoostArchive(type, &r1, oarchiver);
boost::archive::binary_iarchive iarchiver(ss);
LoadInstanceFromBoostArchive(type, &r2, iarchiver);
```

#### 总结

以本例的方式还可实现通过访问器接入静态反射能力. 当然, 不论是接入第三方库的功能, 还是接入未来的标准静态反射, 实现的方式都是常规的 C++ 用法, 这种兼容性体现了 Niflect 框架的原生性

### 例23. 高效序列化

**序列化执行效率起最关键影响的因素为读写格式**

#### Boost

以 boost 为基准, 实现测试框架

```c++
typedef std::vector<int64_t>     Integers;
typedef std::vector<std::string> Strings;

class CRecord
{
public:
	Integers m_ids;
	Strings  m_strings;

	bool operator==(const CRecord& rhs) const
	{
		return m_ids == rhs.m_ids && m_strings == rhs.m_strings;
	}

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int)
	{
		ar & m_ids;
		ar & m_strings;
	}
};

CRecord r1, r2;
for (size_t idx = 0; idx < g_testIntegers.size(); ++idx)
    r1.m_ids.push_back(g_testIntegers[idx]);
for (size_t idx = 0; idx < g_testStringsCount; ++idx)
	r1.m_strings.push_back(g_testStringValue);

std::stringstream ss;
boost::archive::binary_oarchive oarchiver(ss);
oarchiver << r1;
boost::archive::binary_iarchive iarchiver(ss);
iarchiver >> r2;

NIFLECT_ASSERT(r1 == r2);

auto start = std::chrono::high_resolution_clock::now();
for (size_t idx = 0; idx < iterations; ++idx)
{
	std::stringstream ss;
	boost::archive::binary_oarchive oarchiver(ss);
	oarchiver << r1;
	boost::archive::binary_iarchive iarchiver(ss);
	iarchiver >> r2;
}
auto finish = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
std::cout << duration <<std::endl;
```

执行参考耗时为 2.4s

#### NaiveBinary

定义一种最简格式 `NaiveBinary`, 针对数组型的数据读写优化, 关键点在于批量读写同类型数据

```c++
static void StdStreamAosBinaryWrite(std::ostream& os, const std::vector<int64_t>& vec)
{
    auto size = static_cast<Niflect::NifUint32>(vec.size());
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    if (size > 0)
        os.write(reinterpret_cast<const char*>(&vec[0]), sizeof(int64_t) * size);
}
static void StdStreamAosBinaryWrite(std::ostream& os, const std::vector<std::string>& vec)
{
    auto size = static_cast<Niflect::NifUint32>(vec.size());
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    if (size > 0)
    {
        std::vector<Niflect::NifUint32> vecLength;
        vecLength.resize(size);
        for (Niflect::NifUint32 idx = 0; idx < size; ++idx)
            vecLength[idx] = static_cast<Niflect::NifUint32>(vec[idx].size());
        os.write(reinterpret_cast<const char*>(vecLength.data()), sizeof(Niflect::NifUint32) * size);
        for (Niflect::NifUint32 idx = 0; idx < size; ++idx)
            os.write(vec[idx].data(), vecLength[idx]);
    }
}
static void StdStreamAosBinaryRead(std::istream& is, std::vector<int64_t>& vec)
{
    Niflect::NifUint32 size = 0;
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    vec.resize(size);
    if (size > 0)
        is.read(reinterpret_cast<char*>(&vec[0]), sizeof(int64_t) * size);
}
static void StdStreamAosBinaryRead(std::istream& is, std::vector<std::string>& vec)
{
    Niflect::NifUint32 size = 0;
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    vec.resize(size);
    if (size > 0)
    {
        std::vector<Niflect::NifUint32> vecLength;
        vecLength.resize(size);
        is.read(reinterpret_cast<char*>(&vecLength[0]), sizeof(Niflect::NifUint32) * size);
        for (Niflect::NifUint32 idx = 0; idx < size; ++idx)
            vec[idx].resize(vecLength[idx]);
        for (Niflect::NifUint32 idx = 0; idx < size; ++idx)
            is.read(reinterpret_cast<char*>(&vec[idx][0]), vecLength[idx]);
    }
}
```

使用模板元编程区分是否可使用特定类型的优化函数, 如本例中的 `std::vector<int64_t>` 与 `std::vector<std::string>`

```c++
namespace NaiveBinary
{
	template <typename TElem>
	static constexpr bool CanOptimizeArraySerialization()
	{
		return std::is_trivial<TElem>::value;
	}

	template <typename TArrayType>
	class TArrayAccessor : public CNaiveBinaryAccessor
	{
	protected:
		virtual bool SaveImpl(const InstanceType* base, std::ostream& os) const override
		{
			auto& instance = *static_cast<const TArrayType*>(base);
			if (!CCompileTimeOption::s_canOptimize)
			{
				auto elemType = this->GetElementType();
				Niflect::NifUint32 size = static_cast<Niflect::NifUint32>(instance.size());
				os.write(reinterpret_cast<const char*>(&size), sizeof(size));
				for (auto idx = 0; idx < instance.size(); ++idx)
				{
					auto elemBase = &instance[idx];
					SaveInstanceToNaiveBinary(elemType, elemBase, os);
				}
			}
			else
			{
				StdStreamAosBinaryWrite(os, instance);
			}
			return true;
		}
		virtual bool LoadImpl(InstanceType* base, std::istream& is) const override
		{
			auto& instance = *static_cast<TArrayType*>(base);
			if (!CCompileTimeOption::s_canOptimize)
			{
				auto elemType = this->GetElementType();
				Niflect::NifUint32 size = 0;
				is.read(reinterpret_cast<char*>(&size), sizeof(size));
				instance.resize(size);
				for (auto idx = 0; idx < instance.size(); ++idx)
				{
					auto elemBase = &instance[idx];
					LoadInstanceFromNaiveBinary(elemType, elemBase, is);
				}
			}
			else
			{
				StdStreamAosBinaryRead(is, instance);
			}
			return true;
		}

	private:
		class CCompileTimeOption
		{
		public:
			static constexpr bool s_canOptimize = CanOptimizeArraySerialization<typename TArrayType::value_type>();
		};
	};

	template <>
	constexpr bool CanOptimizeArraySerialization<std::string>()
	{
		return true;
	}
}
```

使用反射元数据以 `NaiveBinary` 格式读写

```c++
NIF_T()
class CRecord
{
public:
	bool operator==(const CRecord& rhs) const
	{
		return m_ids == rhs.m_ids && m_strings == rhs.m_strings;
	}

public:
	NIF_F()
	std::vector<int64_t> m_ids;
	NIF_F()
	std::vector<std::string> m_strings;
};

CRecord r1, r2;
for (size_t idx = 0; idx < g_testIntegers.size(); ++idx)
    r1.m_ids.push_back(g_testIntegers[idx]);
for (size_t idx = 0; idx < g_testStringsCount; ++idx)
	r1.m_strings.push_back(g_testStringValue);

Niflect::CNiflectTable table;
InitTable(table);

using namespace NaiveBinary;
auto type = Niflect::StaticGetType<CRecord>();
std::stringstream ss;
SaveInstanceToNaiveBinary(type, &r1, ss);
LoadInstanceFromNaiveBinary(type, &r2, ss);

NIFLECT_ASSERT(r1 == r2);

auto start = std::chrono::high_resolution_clock::now();
for (size_t idx = 0; idx < iterations; ++idx)
{
	std::stringstream ss;
	SaveInstanceToNaiveBinary(type, &r1, ss);
	LoadInstanceFromNaiveBinary(type, &r2, ss);
}
auto finish = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
std::cout << duration <<std::endl;
```

执行参考耗时为 0.6s

##### 基于以动态反射类型擦除方式实现序列化的额外开销说明

对比的基准为纯标准库实现的 `StdStreamAosBinaryWrite/StdStreamAosBinaryRead` 系列函数.

主要在虚函数调用与实例指针转换等, 为了动态能力引入的微小开销, 耗时比基准多约 2~5%

#### 总结

再次**强调**, 此示例**非表明** `NaiveBinary` 比 boost 的序列化执行效率高得多, 此示例仅表明**序列化执行效率起最关键影响的因素为读写格式**, 正如 `NaiveBinary` 的格式命名那样, 示例实现的格式为最简, 因此执行耗时短

另外, 在实践中需要注意的是, 序列化的实现并非一味追求高效, 其实现应充分考虑用途, 例如目前低效实现的 `RwTree`, 其设计目的在于增强反射相关功能的复用性, 以及作为 Niflect 示例的简洁性

### 例24. 通过反射元数据创建实例

定义示例类

```c++
#pragma once
#include "Niflect/Component/DefaultMacroTag.h"
#include <string>

NIF_T()
class CHelloWorld
{
public:
	bool m_bool_0 = false;
	std::string m_str_1 = "Hello";
};
```

以默认堆与默认构造函数为例, 通过反射元数据创建实例

```c++
Niflect::CNiflectType* type = Niflect::StaticGetType<CHelloWorld>();
auto& ConstructFunc = type->m_vecConstructorInfo[0].m_Func;
auto& DestructFunc = type->m_InvokeDestructorFunc;
auto typeSize = type->GetTypeSize();

auto mem = malloc(typeSize);
ConstructFunc(mem, NULL);

auto instance = static_cast<CHelloWorld*>(mem);
printf("%s\n", instance->m_str_1.c_str());

DestructFunc(mem);
free(mem);
```

即使封装为帮助类, 这样的用法既不安全也不实用, 仅为示意构造与析构反射元数据的作用

实际使用可能更希望实例支持所有权管理, 如

```c++
template <typename TBase>
static std::shared_ptr<TBase> MyMakeShared(Niflect::CNiflectType* type)
{
	void* mem = ::operator new(type->GetTypeSize());
	type->m_vecConstructorInfo[0].m_Func(mem, nullptr);
	auto deleter = [type](void* p)
		{
			type->m_InvokeDestructorFunc(p);
			::operator delete(p);
		};
	return std::shared_ptr<TBase>(static_cast<TBase*>(mem), std::move(deleter));
}
```

创建由 `std::shared_ptr` 管理的实例

```c++
Niflect::CNiflectType* type = Niflect::StaticGetType<CHelloWorld>();
std::shared_ptr<CHelloWorld> instance = MyMakeShared<CHelloWorld>(type);
printf("%s\n", instance->m_str_1.c_str());
```

如果希望使用自定义堆内存管理, 可参考 `Niflect::MakeSharedInstance` 相关定义