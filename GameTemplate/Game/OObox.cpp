#include "stdafx.h"
#include "OObox.h"

//�X�^�[�g�֐�
bool OObox::StartSub()
{
	//�������p�֐�
	Init("Assets/modelData/box.tkm", EN_OO_TYPE_BOX);

	//OBB�̃T�C�Y��ݒ�
	Vector3 obbSize;
	obbSize = { 250.0f,250.0f,250.0f };
	GetOBB().SetDirectionLength(obbSize);

	GetOBB().SetTag(COBB::EN_OO_TYPE_BOX);


	return true;
}