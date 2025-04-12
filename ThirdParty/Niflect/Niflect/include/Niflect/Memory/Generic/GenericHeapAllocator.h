#pragma once
#include <cstddef>
#include <utility>

namespace Niflect
{
	template <typename T, typename TMemory>
	class TGenericHeapAllocator
	{
	public:
		typedef T value_type;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;

		typedef T * pointer;
		typedef const T * const_pointer;

		typedef T & reference;
		typedef const T & const_reference;

	public:
		inline TGenericHeapAllocator() throw () { }

		template <typename T2, typename TMemory2>
		inline TGenericHeapAllocator(const TGenericHeapAllocator<T2, TMemory2> &) throw () { }

		inline ~TGenericHeapAllocator() { }

		inline pointer adress(reference r)
		{
			return &r;
		}

		inline const_pointer adress(const_reference r) const
		{
			return &r;
		}

		inline pointer allocate(size_type n)
		{
			return (pointer)TMemory::Alloc(n * sizeof(value_type));
		}

		inline void deallocate(pointer p, size_type)
		{
			TMemory::Free(p);
		}

		//inline void construct(pointer p, const value_type & wert)
		//{
		//	new (p) value_type(wert);
		//}
		template<class _Objty, class ..._Types>
		inline void construct(_Objty *_Ptr, _Types&& ..._Args)
		{	// construct _Objty(_Types...) at _Ptr
			::new ((void *)_Ptr) _Objty(std::forward<_Types>(_Args)...);
		}

		inline void destroy(pointer p)
		{
			p->~value_type();
		}

		inline size_type max_size() const throw ()
		{
			return size_type(-1) / sizeof(value_type);
		}

		template <typename T2, typename TMemory2>
		struct rebind
		{
			typedef TGenericHeapAllocator<T2, TMemory2> other;
		};

		bool operator!=(const TGenericHeapAllocator<T, TMemory>& other) const
		{
			return !(*this == other);
		}

		// Returns true if and only if storage allocated from *this
		// can be deallocated from other, and vice versa.
		// Always returns true for stateless allocators.
		bool operator==(const TGenericHeapAllocator<T, TMemory>& other) const
		{
			return true;
		}
	};
}