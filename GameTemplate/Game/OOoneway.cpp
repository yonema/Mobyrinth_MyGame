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

	//OBBの初期化データ
	SInitOBBData initOBBData;
	initOBBData.width = 10.0f;				//横：X
	initOBBData.length = 600.0f;			//長さ：Z
	initOBBData.height = 400.0f;			//高さ：Y
	initOBBData.pivot = { 0.5f,0.0f,0.5f };	//基点
	initOBBData.rotation = m_rotation;		//回転
	//横へのベクトル
	Vector3 sideVec = g_vec3Right;
	//自身の回転で回す
	m_rotation.Apply(sideVec);
	//横へのベクトルの長さ
	const float sedLen = 300.0f;
	//横へのベクトルを伸ばす
	sideVec.Scale(sedLen);
	//左サイドのOBBの座標
	initOBBData.position = m_position + sideVec;
	//左サイドのOBBの初期化
	m_sideOBB[enLeft].Init(initOBBData);
	//左へのベクトルを右へのベクトルに変換
	sideVec.Scale(-1.0f);
	//右サイドのOBBの座標
	initOBBData.position = m_position + sideVec;
	//右サイドのOBBの初期化
	m_sideOBB[enRight].Init(initOBBData);
	
	//両サイドのOBBをOBBワールドに登録する
	for (int i = 0; i < enLeftAndRightNum; i++)
	{
		COBBWorld::GetInstance()->AddOBB(&m_sideOBB[i]);
	}

#ifdef MY_DEBUG
	Vector3* vertPos;
	for (int i = 0; i < enLeftAndRightNum; i++)
	{
		vertPos = m_sideOBB[i].GetBoxVertex();
		for (int j = 0; j < m_vertNum; j++)
		{
			m_dbgVertMR[i][j] = NewGO<CModelRender>(0);
			m_dbgVertMR[i][j]->Init("Assets/modelData/dbgBox.tkm");
			m_dbgVertMR[i][j]->SetPosition(vertPos[j]);
		}
	}
#endif
	return true;
}

//デストラクタ
OOoneway::~OOoneway()
{
	//両サイドのOBBをOBBワールドから解除する
	for (int i = 0; i < enLeftAndRightNum; i++)
	{
		COBBWorld::GetInstance()->RemoveOBB(&m_sideOBB[i]);
#ifdef MY_DEBUG
		for (int j = 0; j < m_vertNum; j++)
		{
			DeleteGO(m_dbgVertMR[i][j]);
		}
#endif
	}
}

//アップデート関数
void OOoneway::UpdateSub()
{
	//プレイヤーが自身と同じ向きを向いているか
	if (m_pPlayer->GetEnLeftOrRight() == m_leftOrRight)
		//向いている時は
		//通れる
		m_sideOBB[m_leftOrRight].SetExceptionFlag(true);
	else
		//向いていない時は
		//通れない
		m_sideOBB[m_leftOrRight].SetExceptionFlag(false);

}