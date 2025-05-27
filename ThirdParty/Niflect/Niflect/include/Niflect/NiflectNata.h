#pragma once
#include <type_traits>
#include "Niflect/Base/SharedPtr.h"

namespace Niflect
{
	//Native Meta
	class CNata;
	using CSharedNata = TSharedPtr<CNata>;
	class CNata
	{
		template <typename TNata>
		friend CSharedNata MakeDerivedNata(const TNata& derived);

	private:
		virtual bool _DebugFuncForDynamicCast() const { return true; }//仅为动态检查类型避免错误, 如已定义非调试用的virtual函数则可移除, 备注: error C2683: 'dynamic_cast': 'XXX' is not a polymorphic type 
	};

	template <typename TDerivedNata>
	CSharedNata MakeDerivedNata(const TDerivedNata& derived)
	{
		//NIFLECT_ASSERT(derived._DebugFuncForDynamicCast());//检查是否以 CNata 为基类
		static_assert(std::is_base_of<CNata, TDerivedNata>::value, "TDerivedNata must be a type derived from CNata");
		return Niflect::MakeShared<TDerivedNata>(derived);
	}
	template <typename TDerivedNata>
	static TDerivedNata* CastDerivedNata(CNata* base)
	{
		NIFLECT_ASSERT(dynamic_cast<TDerivedNata*>(base) != NULL);
		return static_cast<TDerivedNata*>(base);
	}
}