#include "stdafx.h"
#include "OOwall.h"

bool OOwall::StartSub()
{
	Init("Assets/modelData/wall.tkm", enWall);

	return true;
}

void OOwall::UpdateSub()
{
	if (m_moveFlag)
	{
		Vector3 moveSpeed = g_vec3Up;
		if (m_moveCounter >= 120)
		{
			moveSpeed = g_vec3Down;
			if (m_moveCounter >= 240)
			{
				m_moveCounter = 0;
			}
		}
		m_rotation.Apply(moveSpeed);
		moveSpeed.Scale(5.0f);

		m_position += moveSpeed;
		m_moveCounter++;



	}

	return;
}