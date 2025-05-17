#pragma once
#include "Niflect/Default/BuiltInTypeAccessorSetting.h"
#include "Niflect/Default/CompoundAccessor.h"
#include "Niflect/Default/EnumAccessor.h"

namespace BasicAccessorSetting
{
	using namespace NiflectAccessorSetting;
	using namespace Niflect;

	NIFAS_B() TSetting<CCompoundAccessor>;
	NIFAS_C() TSetting<CEnumClassAccessor>;
	NIFAS_D() TSetting<CEnumBitMaskAccessor>;
}