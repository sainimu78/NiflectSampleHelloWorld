#pragma once
#include "Niflect/NiflectAccessorSetting.h"
#include "Niflect/Default/CompoundAccessor.h"
#include "Niflect/Default/EnumAccessor.h"
#include "Niflect/Default/ValueTypeAccessor.h"
#include "Niflect/Default/ContainerTypeAccessor.h"
#include "Niflect/Default/STLValueTypeAccessor.h"
#include "Niflect/Default/STLContainerTypeAccessor.h"

namespace DefaultAccessorSetting
{
	using namespace NiflectAccessorSetting;
	using namespace Niflect;

	NIFAS_A() TSetting<CBoolAccessor, bool>;
	NIFAS_A() TSetting<CInt32Accessor, int32>;
	NIFAS_A() TSetting<CFloatAccessor, float>;
	NIFAS_A() TSetting<CStringAccessor, Niflect::CString>;
	template <typename TInstance, typename T>
	NIFAS_A() TSetting<TArrayAccessor<TInstance>, Niflect::TArray<T> >;
	//template <typename TInstance, typename T>
	//NIFAS_A() TSetting<TArrayAccessor<TInstance>, Niflect::TArrayNif<T> >;
	template <typename TInstance>
	NIFAS_A() TSetting<TBitsArrayAccessor<TInstance>, Niflect::TArray<bool> >;
	//template <typename TInstance>
	//NIFAS_A() TSetting<TBitsArrayAccessor<TInstance>, Niflect::TArrayNif<bool> >;
	template <typename TInstance, typename T0, typename T1>
	NIFAS_A() TSetting<TMapAccessor<TInstance>, Niflect::TMap<T0, T1>, std::pair<T0, T1> >;
	template <typename T0, typename T1>
	NIFAS_A() TSetting<Niflect::CCompoundAccessor, std::pair<T0, T1> >;
	NIFAS_A() TSetting<CSTLStringAccessor, std::string>;
	template <typename TInstance>
	NIFAS_A() TSetting<TSTLBitsArrayAccessor<TInstance>, std::vector<bool> >;
	template <typename TInstance, typename T>
	NIFAS_A() TSetting<TSTLArrayAccessor<TInstance>, std::vector<T> >;
	template <typename TInstance, typename T0, typename T1>
	NIFAS_A() TSetting<TSTLMapAccessor<TInstance>, std::map<T0, T1>, std::pair<T0, T1> >;
	NIFAS_B() TSetting<CCompoundAccessor>;
	NIFAS_C() TSetting<CEnumClassAccessor>;
	NIFAS_D() TSetting<CEnumBitMaskAccessor>;
}