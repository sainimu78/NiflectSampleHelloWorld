#pragma once
#include "HelloWorld_gen.h"

NIF_T()
class CHelloWorld
{
public:
	CHelloWorld()
		: m_value(0.0f)
	{
	}

public:
	NIF_F()
	float m_value = 0.0f;
};