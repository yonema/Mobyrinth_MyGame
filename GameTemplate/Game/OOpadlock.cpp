#include "stdafx.h"
#include "OOpadlock.h"

//スタート関数
bool OOpadlock::StartSub()
{
	//初期化用関数
	Init("Assets/modelData/padlock1.tkm", enNotHavePadlock);

	//OBBのサイズを設定
	Vector3 obbSize;
	obbSize = { 300.0f,300.0f,300.0f };
	GetOBB().SetDirectionLength(obbSize);

	return true;
}