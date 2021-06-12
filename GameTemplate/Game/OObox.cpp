#include "stdafx.h"
#include "OObox.h"

//スタート関数
bool OObox::StartSub()
{
	//初期化用関数
	Init("Assets/modelData/box.tkm", EN_OO_TYPE_BOX);

	//OBBのサイズを設定
	Vector3 obbSize;
	obbSize = { 250.0f,250.0f,250.0f };
	GetOBB().SetDirectionLength(obbSize);

	GetOBB().SetTag(COBB::EN_OO_TYPE_BOX);


	return true;
}