#include "stdafx.h"
#include "OOpadlock.h"

//�X�^�[�g�֐�
bool OOpadlock::StartSub()
{
	//�������p�֐�
	Init("Assets/modelData/padlock1.tkm", enNotHavePadlock);

	//OBB�̃T�C�Y��ݒ�
	Vector3 obbSize;
	obbSize = { 300.0f,300.0f,300.0f };
	GetOBB().SetDirectionLength(obbSize);

	return true;
}