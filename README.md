![Build Status](https://github.com/sainimu78/NiflectSampleHelloWorld/actions/workflows/Windows.yml/badge.svg)

# Niflect 示例: HelloWorld

**NiflectSampleHelloWorld** 是 Niflect 的最简示例, 可通过该示例体验特性:

- 非侵入式
  - 不要求继承自某个类, 可为第三方库类型生成反射元数据
  - 零运行时开销的声明式反射宏标签, 不改变被反射类型的内存布局与虚表结构
- 任意类型反射与序列化
  - 如嵌套模板, 任意指针等类型
  - 基于可编解码为任意格式的 RwNode 实现保存载入实例
- 动态反射
  - 编译期间通过 **NiflectGenTool** 生成反射元数据
  - 运行时初始化反射元数据

## 示例集

### 1. 最简示例

反射内建类型成员变量



### 2. 反射嵌套模板成员变量

todo