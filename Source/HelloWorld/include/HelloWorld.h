#pragma once
#include "Niflect/Component/DefaultMacroTag.h"
#include "Resource.h"

NIF_T()
class CHelloWorld
{
public:
	NIF_F()
	float m_value = 0.0f;
	NIF_F()
	CResource* m_res = NULL;
};