#include "stdafx.h"
#include "OObox.h"

bool OObox::StartSub()
{
	Init("Assets/modelData/box.tkm", enBox);
	//OBB�̃T�C�Y��ݒ�
	Vector3 obbSize;
	obbSize = { 300.0f,300.0f,300.0f };
	GetOBB().SetDirectionLength(obbSize);

	return true;
}