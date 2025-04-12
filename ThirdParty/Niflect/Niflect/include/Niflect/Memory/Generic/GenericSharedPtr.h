#pragma once
#include "Niflect/NiflectCommon.h"
#include "Niflect/Memory/Generic/GenericInstance.h"
//#define TEST_GENERIC_SHARED_PTR
#ifdef TEST_GENERIC_SHARED_PTR
#include <stdio.h>
#endif

//多使用一个bool变量保存创建来源为MakeShared或MakeSharable, bool变量被对齐为4字节
//为减少此微小空间占用, 也可考虑将 SGenericSharedPtrData 指定为非对齐结构
//现通过假定函数地址是对齐的, 不被使用, 利用最低位保存创建来源
//如果现特殊方法不可行, 或某些平台或编译器不适用, 以启用此宏的实现版本编译即可
#define DETERMINE_CONSTRUCTED_FROM_MAKESHARED_OR_MAKESHARABLE_BY_A_BOOL

#ifdef DETERMINE_CONSTRUCTED_FROM_MAKESHARED_OR_MAKESHARABLE_BY_A_BOOL
#else
//#include <sstream>//打印DestructFunc函数地址
#endif

namespace Niflect
{
#ifdef TEST_GENERIC_SHARED_PTR
	namespace TestSharedPtr
	{
		static void TestReferences();
	}
#endif

	//最简智能指针
	//主要特点说明:
	//1. 可指定Constructor与Destructor回调, 以支持反射系统实例化对象
	//2. 不要求从任何基类继承
	//3. 可通过模板参数TMemory指定内存分配管理
	//4. 严格规定与raw指针建立引用只能通过MakeShared与MkaeSharable
	//5. 引用计数减至0时, 通过MakeSharable建立引用的指针需要释放两个对象, 其一为被引用的对象, 其二为SGenericSharedPtrData引用数据, 通过MakeShared建立引用的指针只需要释放一个对象, 因为被引用对象与SGenericSharedPtrData在同一块内存上
	//6. 移植方便, 如可用于改写某些功能, 使其智能指针的对象也能接受内存管理
	//----------------------------------------------------------------------------------------------
	//与std::shared_ptr不同处:
	//1. std::shared_ptr 在释放时, 通过MakeShared创建的SharedPtr, 不需要删除两个对象, 而std::shared_ptr需要删除引用对象与计数对象
	//2. 不支持week_ptr
	//3. std::shared_ptr 模板实例本身占用空间为2个指针, 而std::shared_ptr包含引用指针与两个引用计数
	//4. std::shared_ptr 由虚函数区分通过 std::make_shared 与 std::shared_ptr 构造建立的引用, 虚函数额外使用8字节, 见 std::_Ref_count_base(即使使用了 VC 中的 __declspec(novtable) 也需要额外的8字节)
	//不考虑任意从原始指针建立引用时, 该类可与std::shared_ptr简单替换
	//----------------------------------------------------------------------------------------------
	//与UE TSharedPtr不同处:
	//1. 删除过程更简单一些, 可直接调用析构函数指针, 而UE TSharedPtr需要通过一个虚函数调用析构函数, 消耗略多一些
	//2. 其它不同处见上述关于std::shared_ptr的第2和3项
	//3. UE TSharedPtr与 VC 的 STL 相同, 通过虚函数区分通过 MakeShared 与 MakeSharable 建立引用, 虚函数额外使用8字节

	struct SGenericSharedPtrData
	{
		uint32 m_refCount;//可改为平台相关的整数或64位整数, 现认为32位足够, 如类型改为64位整数, 则公共智能指针数据占用空间相应增加4字节
#ifdef DETERMINE_CONSTRUCTED_FROM_MAKESHARED_OR_MAKESHARABLE_BY_A_BOOL
		bool m_isAllocatedByMakeShared;//为支持MakeShared, 内部确定如何删除data, 如有规范要求只能用MakeShared或MakeSharable, 则可不定义该bool以减少占用内存与略微简化释放对象的过程
#else
		bool m_debugIsAllocatedByMakeShared;
#endif
		InvokeDestructorFunc m_InvokeDestructorFunc;//为通用性增加该函数指针以支持基类未定义virtual析构也能够安全释放, 如有规范要求基类必须定义virtual析构, 则可不定义该函数指针以减少占用内存
	};

	template <typename TPtr, typename TMemory>
	class TGenericSharedPtr
	{
	public:
		typedef TPtr PtrType;
		typedef TMemory MemoryType;

	private:
		template <typename TDerived, typename TSameMemory>
		friend class TGenericSharedPtr;
		template <typename TBase, typename TMemory2, typename TConstructFunc, typename ...TArgs>
		friend TGenericSharedPtr<TBase, TMemory2> GenericPlacementMakeShared(uint32 objSize, const InvokeDestructorFunc& DestructFunc, const TConstructFunc& ConstructFunc, TArgs&& ...args);
		template <typename TBase, typename TMemory2>
		friend TGenericSharedPtr<TBase, TMemory2> GenericPlacementMakeSharable(TBase* rawPtr, const InvokeDestructorFunc& DestructFunc);

#ifdef TEST_GENERIC_SHARED_PTR
		friend void TestSharedPtr::TestReferences();
#endif

	public:
		TGenericSharedPtr()
			: m_ptr(NULL)
			, m_data(NULL)
		{
		}
		TGenericSharedPtr(const TGenericSharedPtr& rhs)
			: m_ptr(rhs.m_ptr)
			, m_data(rhs.m_data)
		{
			this->IncRef();
		}
		//支持子类赋值到父类指针
		template <typename TDerived, std::enable_if_t<std::is_convertible<TDerived*, PtrType*>::value, int> = 0>
		TGenericSharedPtr(const TGenericSharedPtr<TDerived, MemoryType>& rhs)
			: m_ptr(rhs.m_ptr)
			, m_data(rhs.m_data)
		{
			this->IncRef();
		}
		TGenericSharedPtr(const std::nullptr_t nullPtr)
			: m_ptr(NULL)
			, m_data(NULL)
		{
			ASSERT(nullPtr == nullptr);
		}
		~TGenericSharedPtr()
		{
			this->DecRef();
		}
		template <typename TDerived>
		TGenericSharedPtr<TDerived, MemoryType> Cast() const
		{
			return {static_cast<TDerived*>(m_ptr), m_data};
		}
		template <typename TDerived>
		TGenericSharedPtr<TDerived, MemoryType> CastChecked() const
		{
			return { dynamic_cast<TDerived*>(m_ptr), m_data };
		}

	private:
		TGenericSharedPtr(PtrType* ptr, SGenericSharedPtrData* data)
			: m_ptr(ptr)
			, m_data(data)
		{
			this->IncRef();
		}

	public:
		inline PtrType* Get() const
		{
			return m_ptr;
		}

	public:
		inline TGenericSharedPtr& operator=(const TGenericSharedPtr& rhs)
		{
			if (m_ptr != rhs.m_ptr)
				this->AssignFrom(rhs.m_ptr, rhs.m_data);
			return *this;
		}
		inline PtrType& operator*()
		{
			return *m_ptr;
		}
		inline const PtrType& operator*() const
		{
			return *m_ptr;
		}
		inline PtrType* operator->() const
		{
			return m_ptr;
		}
		inline bool operator==(const TGenericSharedPtr& rhs) const
		{
			return m_ptr == rhs.m_ptr;
		}
		inline bool operator!=(const TGenericSharedPtr& rhs) const
		{
			return !this->operator==(rhs);
		}
		inline bool operator==(const PtrType* ptr) const
		{
			return m_ptr == ptr;
		}
		inline bool operator!=(const PtrType* ptr) const
		{
			return !this->operator==(ptr);
		}
		inline friend bool operator==(const PtrType* ptr, const TGenericSharedPtr& rhs)
		{
			return rhs.operator==(ptr);
		}
		inline friend bool operator!=(const PtrType* ptr, const TGenericSharedPtr& rhs)
		{
			return !(rhs.operator==(ptr));
		}

	public:
		inline explicit operator bool() const
		{
			return m_ptr != NULL;
		}

	private:
		void InitWithData(PtrType* ptr, const InvokeDestructorFunc& DestructFunc)
		{
			ASSERT(m_data == NULL);
			m_ptr = ptr;
			m_data = static_cast<SGenericSharedPtrData*>(MemoryType::Alloc(sizeof(SGenericSharedPtrData)));
			StaticInitData(m_data, DestructFunc, false);
			this->IncRef();
		}
		void InitWithSharedData(PtrType* ptr, const InvokeDestructorFunc& DestructFunc, SGenericSharedPtrData* data)
		{
			ASSERT(m_data == NULL);
			m_ptr = ptr;
			m_data = data;
#ifdef DETERMINE_CONSTRUCTED_FROM_MAKESHARED_OR_MAKESHARABLE_BY_A_BOOL
			StaticInitData(m_data, DestructFunc, true);
#else
			//std::stringstream ss;
			//ss << DestructFunc;
			//printf("My DestructFunc %s\n", ss.str().c_str());
			//测试发现在 MinGW Build 的 DestructFunc 地址为 0x1
			ASSERT((reinterpret_cast<ptrdiff_t>(DestructFunc) & 0x1) == 0);

			auto flaggedDestructFunc = reinterpret_cast<InvokeDestructorFunc>(reinterpret_cast<ptrdiff_t>(DestructFunc) | 0x1);
			StaticInitData(m_data, flaggedDestructFunc, true);
#endif
			this->IncRef();
		}
		inline void IncRef()
		{
			if (m_data != NULL)
				m_data->m_refCount++;
		}
		inline void DecRef()
		{
			if (m_data != NULL)
			{
				m_data->m_refCount--;
				this->DeletePointer();
			}
		}
		inline static void StaticInitData(SGenericSharedPtrData* data, const InvokeDestructorFunc& DestructFunc, bool isAllocatedByMakeShared)
		{
			data->m_refCount = 0u;
			data->m_InvokeDestructorFunc = DestructFunc;
#ifdef DETERMINE_CONSTRUCTED_FROM_MAKESHARED_OR_MAKESHARABLE_BY_A_BOOL
			data->m_isAllocatedByMakeShared = isAllocatedByMakeShared;
#else
			data->m_debugIsAllocatedByMakeShared = isAllocatedByMakeShared;
#endif
		}
		void DeletePointer()
		{
			if (m_data->m_refCount == 0)
			{
#ifdef DETERMINE_CONSTRUCTED_FROM_MAKESHARED_OR_MAKESHARABLE_BY_A_BOOL
				if (m_data->m_isAllocatedByMakeShared)
				{
					m_data->m_InvokeDestructorFunc(m_ptr);
					auto mem = reinterpret_cast<char*>(m_ptr) - sizeof(SGenericSharedPtrData);
					MemoryType::Free(mem);
				}
				else
				{
					CGenericInstance::DestructAndFree<MemoryType>(m_ptr, m_data->m_InvokeDestructorFunc);
					//UE的实现方法, 定义一种析构代理类(见DefaultDeleter), MakeShareable时使用CRT默认new创建, 在删除时使用的CRT默认delete释放
					MemoryType::Free(m_data);
				}
#else
				//虽然是连续内存, MkaeShared 的 m_ptr 地址一定比 m_data 大, 但隐患在于 m_data 可能是在一些分配和释放操作后再分配的, 就有一定可能性刚比 m_ptr 小
				//if (reinterpret_cast<ptrdiff_t>(m_ptr) > reinterpret_cast<ptrdiff_t>(m_data))
				//{
				//	...
				//	void* mem = reinterpret_cast<char*>(m_ptr) - sizeof(SGenericSharedPtrData);
				//	if (mem == m_data)
				//		...
				//}
				
				//据说编译器能保证函数地址是对齐的, 因此最低位不使用
				//begin, GPT解释
				//对于指针对齐的相关条款:
				//	<cstdint> 和类型对齐 :
				//		C++标准中的 alignas 和 alignof 关键字可以帮助您控制和查询类型的对齐要求，以便您知道如何处理不同类型的指针。
				//	函数的对齐要求 :
				//		虽然C++标准没有明确规定函数指针的对齐方式，但通常情况下，编译器会确保函数指针的对齐与数据指针相同，以便在实际使用中避免问题。
				//	相关链接 :
				//		虽然没有直接的链接可以证明函数指针必须对齐，但以下是一些C++标准文档的相关参考：
				//			C++ Standard(ISO / IEC 14882)（可以在此查找最新的C++标准草案和详细说明）
				//			C++ Core Guidelines（这些指南常常强调类型安全和内存管理的重要性）
				//end

				auto funcAsInt = reinterpret_cast<ptrdiff_t>(m_data->m_InvokeDestructorFunc);
				if ((funcAsInt & 0x1) != 0)
				{
					funcAsInt &= ~0x1;
					void* mem = reinterpret_cast<char*>(m_ptr) - sizeof(SGenericSharedPtrData);
					ASSERT(m_data->m_debugIsAllocatedByMakeShared);
					auto DestructorFunc = reinterpret_cast<InvokeDestructorFunc>(funcAsInt);
					DestructorFunc(m_ptr);
					MemoryType::Free(mem);
					return;
				}

				ASSERT((reinterpret_cast<ptrdiff_t>(m_data->m_InvokeDestructorFunc) & 0x1) == 0);
				ASSERT(!m_data->m_debugIsAllocatedByMakeShared);

				CGenericInstance::DestructAndFree<MemoryType>(m_ptr, m_data->m_InvokeDestructorFunc);
				//UE的实现方法, 定义一种析构代理类(见DefaultDeleter), MakeShareable时使用CRT默认new创建, 在删除时使用的CRT默认delete释放
				MemoryType::Free(m_data);
#endif
			}
		}
		inline void AssignFrom(PtrType* ptr, SGenericSharedPtrData* data)
		{
			this->DecRef();
			m_ptr = ptr;
			m_data = data;
			this->IncRef();
		}

	private:
		template <typename TDerived>
		inline static bool StaticCheck(TDerived& rhsRawPtr)
		{
			const PtrType* thisRawPtr = rhsRawPtr;//静态检查是否为安全指针转换, 如父类转子类指针为不安全, 应先正确cast后再赋TSharedPtr
			return true;
		}

	private:
		PtrType* m_ptr;
		SGenericSharedPtrData* m_data;
	};
	template <typename TBase, typename TMemory, typename TConstructFunc, typename ...TArgs>
	static TGenericSharedPtr<TBase, TMemory> GenericPlacementMakeShared(uint32 objSize, const InvokeDestructorFunc& DestructFunc, const TConstructFunc& ConstructFunc, TArgs&& ...args)
	{
		TGenericSharedPtr<TBase, TMemory> shared;
		auto dataSize = sizeof(SGenericSharedPtrData);
		auto mem = TMemory::Alloc(dataSize + objSize);
		auto obj = reinterpret_cast<TBase*>(static_cast<char*>(mem) + dataSize);
		ConstructFunc(obj, std::forward<TArgs>(args)...);
		shared.InitWithSharedData(obj, DestructFunc, static_cast<SGenericSharedPtrData*>(mem));
		return shared;
	}
	template <typename TDerived, typename TMemory, typename ...TArgs>
	static TGenericSharedPtr<TDerived, TMemory> GenericMakeShared(TArgs&& ...args)
	{
		return GenericPlacementMakeShared<TDerived, TMemory>(sizeof(TDerived), &GenericInstanceInvokeDestructor<TDerived>, &GenericInstanceInvokeConstructor<TDerived, TArgs...>, std::forward<TArgs>(args)...);
	}
	//当基类未定义virtual析构时, 为确保安全释放子类对象, 应使用MakeSharable, 在创建原始子类raw指针后立即建立引用
	//如果能确保基类定义virtual析构, 则直接使用构造函数或=都是能安全释放的
	template <typename TBase, typename TMemory>
	static TGenericSharedPtr<TBase, TMemory> GenericPlacementMakeSharable(TBase* rawPtr, const InvokeDestructorFunc& DestructFunc)
	{
		TGenericSharedPtr<TBase, TMemory> shared;
		ASSERT(rawPtr != NULL);
		shared.InitWithData(rawPtr, DestructFunc);
		return shared;
	}
	template <typename TDerived, typename TMemory>
	static TGenericSharedPtr<TDerived, TMemory> GenericMakeSharable(TDerived* rawPtr)
	{
		return GenericPlacementMakeSharable<TDerived, TMemory>(rawPtr, &GenericInstanceInvokeDestructor<TDerived>);
	}

#ifdef TEST_GENERIC_SHARED_PTR
	namespace TestSharedPtr
	{
		class CMyClass0
		{
		public:
			CMyClass0()
				: m_myFloat0(111.0f)
				, m_myFloat1(111.1f)
			{
				static int cnt = 0;
				printf("CMyClass0, constructor, %d\n", cnt++);
			}
			~CMyClass0()
			{
				static int cnt = 0;
				printf("CMyClass0, destructor, %d\n", cnt++);
			}

		public:
			float m_myFloat0;
			float m_myFloat1;
		};
		class CMyClassBase1
		{
		public:
			CMyClassBase1()
				: m_myFloatBase2(111.2f)
				, m_myFloatBase3(111.3f)
			{
				static int cnt = 0;
				printf("CMyClassBase1, constructor, %d\n", cnt++);
			}
			~CMyClassBase1()
			{
				static int cnt = 0;
				printf("CMyClassBase1, destructor, %d\n", cnt++);
			}

		public:
			float m_myFloatBase2;
			float m_myFloatBase3;
		};
		class CMyClass1 : public CMyClassBase1
		{
		public:
			CMyClass1()
				: m_myFloat4(111.4f)
				, m_myFloat5(111.5f)
			{
				static int cnt = 0;
				printf("CMyClass1, constructor, %d\n", cnt++);
			}
			CMyClass1(float arg0, bool arg1)
				: CMyClass1()
			{
			}
			~CMyClass1()
			{
				static int cnt = 0;
				printf("CMyClass1, destructor, %d\n", cnt++);
			}

		public:
			float m_myFloat4;
			CMyClass0 m_sub0;
			float m_myFloat5;
		};
		static size_t s_runtimeBytes = 0;
		
		class CTestMemory
		{
		public:
			static void* Alloc(size_t size)
			{
				auto actual = sizeof(size_t) + size;
				s_runtimeBytes += size;
				auto mem = malloc(actual);
				*static_cast<size_t*>(mem) = size;
				return static_cast<char*>(mem) + sizeof(size_t);
			}
			static void* Realloc(void* ptr, size_t size)
			{
				ASSERT(false);
				return realloc(ptr, size);
			}
			static void Free(void* ptr)
			{
				void* mem = (static_cast<char*>(ptr) - sizeof(size_t));
				const auto& size = *static_cast<size_t*>(mem);
				s_runtimeBytes -= size;
				free(mem);
			}
		};

		template <typename T>
		using TTestSharedPtr = TGenericSharedPtr<T, CTestMemory>;

		template <typename T>
		inline static TTestSharedPtr<T> TestMakeSharable(T* rawPtr)
		{
			return GenericPlacementMakeSharable<T, CTestMemory>(rawPtr, &GenericInstanceInvokeDestructor<T>);
		}
		template <typename T, typename ...TArgs>
		inline static TTestSharedPtr<T> TestMakeShared(TArgs&& ...args)
		{
			return GenericPlacementMakeShared<T, CTestMemory>(sizeof(T), &GenericInstanceInvokeDestructor<T>, &GenericInstanceInvokeConstructor<T, TArgs...>, std::forward<TArgs>(args)...);
		}

		static void TestSharedPtrConstRef(const TTestSharedPtr<CMyClass1>& obj)
		{
		}
		static void TestSharedPtrRef(TTestSharedPtr<CMyClass1>& obj)
		{
		}
		static void TestSharedPtrCopy(TTestSharedPtr<CMyClass1> obj)
		{
		}
		static void TestSharedPtrRefForError(TTestSharedPtr<CMyClass1>& obj)
		{
		}

		static void TestForCompiling()
		{
			{
				//auto a = sizeof(std::shared_ptr<float>);
				//auto b = sizeof(TGenericSharedPtrData<float>);
				//auto c = sizeof(TGenericSharedPtr<float, MemoryType>);
				//auto e = sizeof(std::aligned_union<1, float>::type);
				//std::shared_ptr<float> dddddd;
				//auto g = sizeof(dddddd.use_count());
				//printf("");
			}
			//begin, TSharedPtr测试
			TTestSharedPtr<CMyClass1> a;
			a = NULL;
			a = nullptr;
			TTestSharedPtr<CMyClass1> b;
			a = b;
			TTestSharedPtr<CMyClass1> c = a;
			TTestSharedPtr<CMyClassBase1> d = a;
			TTestSharedPtr<CMyClassBase1> e;
			e = a;
			TestSharedPtrConstRef(NULL);
			TestSharedPtrConstRef(nullptr);
			TestSharedPtrConstRef(a);
			TestSharedPtrRef(a);
			TestSharedPtrCopy(NULL);
			TestSharedPtrCopy(nullptr);
			TestSharedPtrCopy(a);
			//启用以下宏后应出现编译错误, 即不允许直接通过raw指针赋值
	//#define TEST_FOR_ERROR
	#ifdef TEST_FOR_ERROR
			CMyClass1* f = NULL;
			a = f;
			TSharedPtr<CMyClass1> g = f;
			TestSharedPtrRefForError(NULL);
			TestSharedPtrRefForError(nullptr);
	#endif
			//end
		}
		void TestReferences()
		{
			auto& runtimeBytes = s_runtimeBytes;
			{
				TTestSharedPtr<CMyClass1> a = TestMakeShared<CMyClass1>();
				auto sizeWithSharedPtrUnit = sizeof(CMyClass1)+sizeof(TTestSharedPtr<CMyClass1>);
				ASSERT(a.m_data->m_refCount == 1);
				ASSERT(runtimeBytes == sizeWithSharedPtrUnit);
				auto b = TestMakeShared<CMyClass1>(1.0f, true);
				ASSERT(runtimeBytes == sizeWithSharedPtrUnit * 2);
				ASSERT(b.m_data->m_refCount == 1);
				b = a;
				ASSERT(a.m_data->m_refCount == 2);
				ASSERT(a.m_data == b.m_data);
				auto c = b;
				ASSERT(a.m_data->m_refCount == 3);
				b = NULL;
				ASSERT(a.m_data->m_refCount == 2);
				ASSERT(runtimeBytes == sizeWithSharedPtrUnit);
				CMyClass1* d = new (CTestMemory::Alloc(sizeof(CMyClass1))) CMyClass1();
				ASSERT(runtimeBytes == sizeWithSharedPtrUnit + sizeof(CMyClass1));
				auto e = TestMakeSharable(d);
				ASSERT(e.m_data->m_refCount == 1);
				ASSERT(runtimeBytes == sizeWithSharedPtrUnit * 2);
				auto f = e;
				ASSERT(e.m_data->m_refCount == 2);
				ASSERT(runtimeBytes == sizeWithSharedPtrUnit * 2);
				f = NULL;
				ASSERT(e.m_data->m_refCount == 1);
				e = nullptr;
				ASSERT(e.m_data == NULL);
				ASSERT(runtimeBytes == sizeWithSharedPtrUnit);
			}
			ASSERT(runtimeBytes == 0);
		}
	}
#endif
}