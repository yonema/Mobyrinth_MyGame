#include "stdafx.h"
#include "OObox.h"

//�X�^�[�g�֐�
bool OObox::StartSub()
{
	//�������p�֐�
	Init("Assets/modelData/box.tkm", enBox);

	//OBB�̃T�C�Y��ݒ�
	Vector3 obbSize;
	obbSize = { 250.0f,250.0f,250.0f };
	GetOBB().SetDirectionLength(obbSize);

	GetOBB().SetTag(COBB::enBox);


	return true;
}