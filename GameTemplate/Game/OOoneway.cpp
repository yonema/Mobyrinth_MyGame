#include "stdafx.h"
#include "OOoneway.h"

//スタート関数
bool OOoneway::StartSub()
{
	//モデルの初期化とタイプの設定
	Init("Assets/modelData/oneway.tkm", enOneway);

	//OBBのサイズを設定
	Vector3 obbSize;
	obbSize = { 300.0f,300.0f,400.0f };
	//OBBの方向ベクトルの長さを設定
	GetOBB().SetDirectionLength(obbSize);

	//OBBWorldに自身のOBBの登録を消去させる
	COBBWorld::GetInstance()->RemoveOBB(&GetOBB());


	SInitOBBData initOBBData;
	initOBBData.width = 10.0f;
	initOBBData.length = 400.0f;
	initOBBData.height = 400.0f;
	initOBBData.pivot = { 0.5f,0.0f,0.5f };
	Vector3 sideVec = g_vec3Right;
	m_rotation.Apply(sideVec);
	const float sedLen = 300.0f;
	sideVec.Scale(sedLen);
	initOBBData.position = m_position + sideVec;
	m_sideOBB[enLeft].Init(initOBBData);

	return true;
}

//デストラクタ
OOoneway::~OOoneway()
{

}

//アップデート関数
void OOoneway::UpdateSub()
{

}