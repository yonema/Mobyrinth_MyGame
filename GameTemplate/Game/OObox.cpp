#include "stdafx.h"
#include "OObox.h"

bool OObox::StartSub()
{
	Init("Assets/modelData/box.tkm", enBox);
	//OBBのサイズを設定
	Vector3 obbSize;
	obbSize = { 200.0f,200.0f,200.0f };
	GetOBB().SetDirectionLength(obbSize);

	return true;
}