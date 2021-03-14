#include "stdafx.h"
#include "OOpadlock.h"

bool OOpadlock::StartSub()
{
	Init("Assets/modelData/padlock.tkm", enBigPadlock);

	//m_scale = { 2.0f,2.0f,2.0f };

	return true;
}