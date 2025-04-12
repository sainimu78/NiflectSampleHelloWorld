#pragma once
#include <new>//Placement new

namespace Niflect
{
	typedef void (*InvokeConstructorFunc)(void* instance);
	typedef void (*InvokeDestructorFunc)(void* instance);
	
	//template <typename TClass>
	//static void GenericInstanceInvokeConstructor(void* obj)
	//{
	//	new (obj) TClass();//为兼容built in类型, 使用placement new调用构造函数
	//}
	
	//该模板函数取地址, 可兼容有参数和无参数的函数指针类型
	template <typename TType, typename ...TArgs>
	inline static void GenericInstanceInvokeConstructor(void* instance, TArgs&& ...args)
	{
		new (instance) TType(args...);
	}

	template <typename TType>
	inline static void GenericInstanceInvokeDestructor(void* instance)
	{
		static_cast<TType*>(instance)->~TType();
	}
	
	class CGenericInstance
	{
	public:
		template <typename TMemory, typename TType, typename ...TArgs>
		inline static TType* New(TArgs&& ...args)
		{
			auto mem = TMemory::Alloc(sizeof(TType));
			GenericInstanceInvokeConstructor<TType>(mem, std::forward<TArgs>(args)...);
			return static_cast<TType*>(mem);
		}
		template <typename TMemory, typename TType>
		inline static void Delete(TType* obj)
		{
			GenericInstanceInvokeDestructor<TType>(obj);
			TMemory::Free(obj);
		}
		template <typename TMemory>
		inline static void DestructAndFree(void* obj, const InvokeDestructorFunc& Func)
		{
			Func(obj);
			TMemory::Free(obj);
		}
	};
}

//namespace Reflector
//{
//	//可实现基类无virtual析构也能安全Delete基类指针, 但实际上此问题原因为编码不规范导致, 并不是一种实际需求, 详见该类Delete函数中的说明
//	template <typename TMemory>
//	class CClassMemorySafe
//	{
//	private:
//		typedef void(*PlacementDeleteFunc)(void* obj);
//		struct SPlacementObjectInfo
//		{
//			PlacementDeleteFunc m_DeleteFunc;
//		};
//		template <typename TClass>
//		inline static void PlacementDeleteGeneric(void* obj)
//		{
//			auto casted = static_cast<TClass*>(obj);
//			casted->~TClass();
//		}
//
//	public:
//		template <typename TClass, typename ...TArgs>
//		static TClass* New(TArgs&& ...args)
//		{
//			const auto placementInfoSize = sizeof(SPlacementObjectInfo);
//			const auto objectSize = sizeof(TClass);
//			auto mem = static_cast<char*>(TMemory::Alloc(placementInfoSize + objectSize));
//			auto placementInfo = reinterpret_cast<SPlacementObjectInfo*>(mem);
//			placementInfo->m_DeleteFunc = &PlacementDeleteGeneric<TClass>;
//			TClass* obj = new (mem + placementInfoSize) TClass(args...);
//			return obj;
//		}
//		template <typename TClass>
//		static void Delete(TClass* obj)
//		{
//			if (obj == NULL)
//				return;
//			const auto placementInfoSize = sizeof(SPlacementObjectInfo);
//			auto mem = reinterpret_cast<char*>(obj) - placementInfoSize;
//			auto placementInfo = reinterpret_cast<SPlacementObjectInfo*>(mem);
//			//模板参数TClass可能为基类, 子类未定义override析构时, 显式调用基类的析构导致子类不能完全析构, 如子类有其它类时可观察到其它类不会被析构
//			//因此通过函数指针在New阶段记录正确的类析构再在delete阶段调用可实现安全delete placement new的对象, 不必都对子类定义override析构
//			//缺点在于多增加函数指针空间占用
//			placementInfo->m_DeleteFunc(obj);
//			TMemory::Free(mem);
//		}
//	};
//}