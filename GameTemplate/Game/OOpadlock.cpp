#include "stdafx.h"
#include "OOpadlock.h"

bool OOpadlock::StartSub()
{
	Init("Assets/modelData/padlock.tkm", enNotHavePadlock);

	//OBB�̃T�C�Y��ݒ�
	Vector3 obbSize;
	obbSize = { 300.0f,300.0f,300.0f };
	GetOBB().SetDirectionLength(obbSize);

	return true;
}