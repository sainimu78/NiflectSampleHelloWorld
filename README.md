![Build Status Linux](https://github.com/sainimu78/NiflectSampleHelloWorld/actions/workflows/Linux.yml/badge.svg)
![Build Status Windows](https://github.com/sainimu78/NiflectSampleHelloWorld/actions/workflows/Windows.yml/badge.svg)

# Niflect 示例: HelloWorld

NiflectSampleHelloWorld 是最简示例项目, 用于帮助使用者掌握 C++ 原生风格反射框架 **Niflect** 的用法.

## Niflect 的特性

### 非侵入式

- 不要求继承自某个类
- 零运行时开销声明式反射宏标签, 不改变被反射类型内存布局与虚表结构. 即被反射前后 `sizeof` 值相同
- 可为第三方库类型字段生成反射元数据

### C++ 原生风格

- 支持通过非默认构造函数实例化. 仅要求构造函数的参数为可被反射的字段类型
- 原生风格的元数据绑定, 即 IDE 友好且类型安全. 字段绑定的元数据即为 C++ 原生代码定义的类, 而非难使用, 难维护且须解析的字符串
- 字段类型可为别名

### 反射与序列化

- 支持可实例化类型定义与其几乎任意类型字段的反射与序列化
  - 多层嵌套的任意容器模板字段, 如 `std::vector<std::map<std::string, int> >`
  - 任意指针模板, 任意原始指针等字段, 仅须实现自定的序列化方法
- 基于可编解码为任意格式的树型结构 `RwNode` 实现保存载入实例, 格式如 Niflect 提供的 JSON 格式编解码

### 动态反射

- 构建开始时通过 **NiflectGenTool** 生成反射代码, 在运行时仅要求在使用前初始化反射元数据
- 可跨模块使用反射元数据
- 通过反射元数据
  - 实例化对象, 无需依赖类型定义所在头文件
  - 实现 AoS 到 SoA 的动态自动映射
  - 实现内存布局, 虚表无关的实例替换

## 示例集

todo: 目录

### 最简示例

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

#### 初始化与使用反射元数据

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

### 反射字段

float

std::vector

### `RwNode` 保存载入与序列化

todo

### 反射嵌套模板字段

todo

### 任意自定义类型字段序列化

todo

### 反射原始指针字段

todo

### 不依赖类型定义头文件的实例化

todo

### 自定义反射宏标签名称

todo

### 反射私有字段

todo

### 通过实例获取反射元数据

todo

### Niflect 提供的简易内存管理

TSharedPtr

MemoryStats

### 模块反射元数据自动发现

todo

### 跨模块使用反射元数据

todo

### 反射成员函数与静态函数

todo

### 反射全局变量与全局函数

todo

### 反射非默认构造函数

todo

### 将字段绑定原生类编写表示的元数据

todo

### 遍历反射元数据

todo

### 绑定别名字段类型

todo