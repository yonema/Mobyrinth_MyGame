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
	m_position = initData.position;
	m_directionLength[enLocalX] = initData.width / 2;
	m_directionLength[enLocalY] = initData.length / 2;
	m_directionLength[enLocalZ] = initData.height / 2;

	Rotating(initData.rotation);

	//m_normalDirection[enLocalX] = initData.
}
void COBB::Positioning(const Vector3& pos)
{

}

void COBB::Rotating(const Quaternion& rot)
{
	m_normalDirection[enLocalX] = g_vec3Right;
	m_normalDirection[enLocalY] = g_vec3Up;
	m_normalDirection[enLocalZ] = g_vec3Front;
	rot.Apply(m_normalDirection[enLocalX]);
	rot.Apply(m_normalDirection[enLocalY]);
	rot.Apply(m_normalDirection[enLocalZ]);

	return;
}