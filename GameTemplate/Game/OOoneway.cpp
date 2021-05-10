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
	initOBBData.length = 600.0f;
	initOBBData.height = 400.0f;
	initOBBData.pivot = { 0.5f,0.0f,0.5f };
	initOBBData.rotation = m_rotation;
	Vector3 sideVec = g_vec3Right;
	m_rotation.Apply(sideVec);
	const float sedLen = 300.0f;
	sideVec.Scale(sedLen);
	initOBBData.position = m_position + sideVec;
	m_sideOBB[enLeft].Init(initOBBData);
	sideVec.Scale(-1.0f);
	initOBBData.position = m_position + sideVec;
	m_sideOBB[enRight].Init(initOBBData);

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
		if (i == m_leftOrRight)
			COBBWorld::GetInstance()->AddOBB(&m_sideOBB[i]);
	}

	return true;
}

//デストラクタ
OOoneway::~OOoneway()
{
	for (int i = 0; i < enLeftAndRightNum; i++)
	{
		COBBWorld::GetInstance()->RemoveOBB(&m_sideOBB[i]);

		for (int j = 0; j < m_vertNum; j++)
		{
			DeleteGO(m_dbgVertMR[i][j]);
		}
	}
}

//アップデート関数
void OOoneway::UpdateSub()
{

		if (m_pPlayer->GetEnLeftOrRight() == m_leftOrRight)
			m_sideOBB[m_leftOrRight].SetExceptionFlag(true);
		else
			m_sideOBB[m_leftOrRight].SetExceptionFlag(false);


}