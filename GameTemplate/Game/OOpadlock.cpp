#include "stdafx.h"
#include "OOpadlock.h"

//�X�^�[�g�֐�
bool OOpadlock::StartSub()
{
	//�������p�֐�
	Init("Assets/modelData/padlock1.tkm", EN_OO_TYPE_BIG_PADLOCK);

	//OBB�̃T�C�Y��ݒ�
	Vector3 obbSize;
	obbSize = { 300.0f,300.0f,300.0f };
	GetOBB().SetDirectionLength(obbSize);

	GetOBB().SetTag(COBB::EN_RO_TYPE_PADLOCK);

	return true;
}