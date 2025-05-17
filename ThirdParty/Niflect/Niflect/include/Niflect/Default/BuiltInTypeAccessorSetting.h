#pragma once
#include "Niflect/NiflectAccessorSetting.h"
#include "Niflect/Default/ValueTypeAccessor.h"

namespace BuiltInAccessorSetting
{
	using namespace NiflectAccessorSetting;
	using namespace Niflect;

	NIFAS_A() TSetting<CBoolAccessor, bool>;
	NIFAS_A() TSetting<CInt32Accessor, int>;
	NIFAS_A() TSetting<CFloatAccessor, float>;
}