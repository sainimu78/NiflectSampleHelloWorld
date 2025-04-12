#pragma once
#include "Niflect/NiflectCommon.h"

namespace Niflect
{
	class CNiflectType;

	template <typename T>
	class TRegisteredType
	{
		template <typename T2>
		friend CNiflectType* StaticGetTypeOld();

		template <typename T2>
		friend CNiflectType* StaticGetType();
		friend class CNiflectTable;

	public:
		static bool IsValid()
		{
			return s_type != NULL;
		}

	private:
		static CNiflectType* s_type;
	};

	template <typename T>
	CNiflectType* TRegisteredType<T>::s_type = NULL;

	template <typename T>
	CNiflectType* StaticGetTypeOld()
	{
		return TRegisteredType<T>::s_type;
	}

	template <typename T>
	CNiflectType* StaticGetType();
}