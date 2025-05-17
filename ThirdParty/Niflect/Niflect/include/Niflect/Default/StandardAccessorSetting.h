#pragma once
#include "Niflect/Default/BasicAccessorSetting.h"
#include "Niflect/Default/STLValueTypeAccessor.h"
#include "Niflect/Default/STLContainerTypeAccessor.h"

namespace StandardAccessorSetting
{
	using namespace NiflectAccessorSetting;
	using namespace Niflect;

	template <typename T0, typename T1>
	NIFAS_A() TSetting<CCompoundAccessor, std::pair<T0, T1> >;
	NIFAS_A() TSetting<CSTLStringAccessor, std::string>;
	template <typename TInstance>
	NIFAS_A() TSetting<TSTLBitsArrayAccessor<TInstance>, std::vector<bool> >;
	template <typename TInstance, typename T>
	NIFAS_A() TSetting<TSTLArrayAccessor<TInstance>, std::vector<T> >;
	template <typename TInstance, typename T0, typename T1>
	NIFAS_A() TSetting<TSTLMapAccessor<TInstance>, std::map<T0, T1>, std::pair<T0, T1> >;
}