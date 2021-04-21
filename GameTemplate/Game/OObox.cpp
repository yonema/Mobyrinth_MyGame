#include "stdafx.h"
#include "OObox.h"

//スタート関数
bool OObox::StartSub()
{
	//初期化用関数
	Init("Assets/modelData/box.tkm", enBox);

	//OBBのサイズを設定
	Vector3 obbSize;
	obbSize = { 250.0f,250.0f,250.0f };
	GetOBB().SetDirectionLength(obbSize);

	GetOBB().SetTag(COBB::enBox);


	return true;
}