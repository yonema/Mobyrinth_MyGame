#include "stdafx.h"
#include "OOpadlock.h"

//スタート関数
bool OOpadlock::StartSub()
{
	//初期化用関数
	Init("Assets/modelData/padlock1.tkm", EN_OO_TYPE_BIG_PADLOCK);

	//OBBのサイズを設定
	Vector3 obbSize;
	obbSize = { 300.0f,300.0f,300.0f };
	GetOBB().SetDirectionLength(obbSize);

	GetOBB().SetTag(COBB::EN_RO_TYPE_PADLOCK);

	return true;
}