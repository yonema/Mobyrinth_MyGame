#include "stdafx.h"
#include "OObigFire.h"


bool OObigFire::StartSub()
{
	Init("Assets/modelData/bigFire.tkm", enBigFire);

	return true;
}

void OObigFire::Damage()
{
	if (--m_hp <= 0)
	{
		DeleteGO(this);
	}
	else
	{
		SetScale(g_vec3One * m_hp / m_maxHp);
	}
}