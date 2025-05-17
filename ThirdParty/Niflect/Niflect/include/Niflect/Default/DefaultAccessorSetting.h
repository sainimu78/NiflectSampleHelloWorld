#pragma once
#include "Niflect/Default/StandardAccessorSetting.h"

namespace Niflect
{
	using CStringAccessor = TValueTypeAccessor<Niflect::CString>;
}

namespace DefaultAccessorSetting
{
	using namespace NiflectAccessorSetting;
	using namespace Niflect;

	NIFAS_A() TSetting<Niflect::CStringAccessor, Niflect::CString>;
	template <typename TInstance, typename T>
	NIFAS_A() TSetting<Niflect::TArrayAccessor<TInstance>, Niflect::TArray<T> >;
	template <typename TInstance>
	NIFAS_A() TSetting<Niflect::TBitsArrayAccessor<TInstance>, Niflect::TArray<bool> >;
	template <typename TInstance, typename T0, typename T1>
	NIFAS_A() TSetting<Niflect::TMapAccessor<TInstance>, Niflect::TMap<T0, T1>, std::pair<T0, T1> >;
}