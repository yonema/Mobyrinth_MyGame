#include "stdafx.h"
#include "OBB.h"

COBB::COBB()
{
	m_position = g_vec3Zero;
	m_normalDirection[enLocalX] = g_vec3Right;
	m_normalDirection[enLocalY] = g_vec3Up;
	m_normalDirection[enLocalZ] = g_vec3Front;

	for (int i = 0; i < enLocalAxisNum; i++)
	{
		m_directionLength[i] = 1.0f;
	}

}

void COBB::Init(SInitOBBData& initData)
{
	
}