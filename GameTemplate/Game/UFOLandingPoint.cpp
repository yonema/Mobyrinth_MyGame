#include "stdafx.h"
#include "UFOLandingPoint.h"

//スタート関数
bool CUFOLandingPoint::StartSub()
{
	//モデルの初期化とタイプの設定
	Init("Assets/modelData/grilled_chicken.tkm", enEmpty);

	

	//OBBWorldに自身のOBBの登録を消去させる
	COBBWorld::GetInstance()->RemoveOBB(&GetOBB());

	//自己発光色を設定
	SetEmissionColor({ 0.5f,0.5f,0.0f,1.0f });

	GetOBB().SetDirectionLength({ 2000.0f,750.0f,500.0f });
	GetOBB().SetPivot({ 0.5f,0.0f,0.5f });

	for (int i = 0; i < enLeftAndRightNum; i++)
	{
		m_sideOBB[i].SetDirectionLength({ 10.0f,750.0f,500.0f });
		m_sideOBB[i].SetPivot({ 0.5f,0.0f,0.5f });
		m_sideOBB[i].SetPosition(m_position);
		m_sideOBB[i].SetRotation(m_rotation);
	}




	//デバック用
	//後で消す
	Vector3* vertPos = GetOBB().GetBoxVertex();
	for (int i = 0; i < m_vertNum; i++)
	{
		//OBBの頂点を見るためのモデル
		m_dbgVertPosMR[i] = NewGO<CModelRender>(0);
		m_dbgVertPosMR[i]->Init("Assets/modelData/dbgBox.tkm");
		m_dbgVertPosMR[i]->SetPosition(vertPos[i]);
		m_dbgVertPosMR[i]->SetRotation(m_rotation);
	}

	for (int i = 0; i < enLeftAndRightNum; i++)
	{
		vertPos = m_sideOBB[i].GetBoxVertex();
		for (int j = 0; j < m_vertNum; j++)
		{
			m_dbgSidePosMR[i][j] = NewGO<CModelRender>(0);
			m_dbgSidePosMR[i][j]->Init("Assets/modelData/dbgBox.tkm");
			m_dbgSidePosMR[i][j]->SetPosition(vertPos[j]);
			m_dbgSidePosMR[i][j]->SetRotation(m_rotation);
		}
	}
	//デバック用ここまで

	UpdateSideOBB();

	return true;
}


void CUFOLandingPoint::UpdateSideOBB()
{
	const float sideLen = 180.0f;

	Vector3 sideVec[enLeftAndRightNum] = { g_vec3Left, g_vec3Right };

	for (int i = 0; i < enLeftAndRightNum; i++)
	{
		m_rotation.Apply(sideVec[i]);
		sideVec[i].Scale(sideLen);
		sideVec[i].Scale(-1.0f);


		m_sideOBB[i].SetPosition(m_position + sideVec[i]);
		m_sideOBB[i].SetRotation(m_rotation);
	}
	//デバック用
	//後で消す
	for (int i = 0; i < enLeftAndRightNum; i++)
	{
		Vector3* vertPos = m_sideOBB[i].GetBoxVertex();
		for (int j = 0; j < m_vertNum; j++)
		{
			m_dbgSidePosMR[i][j]->SetPosition(vertPos[j]);
			m_dbgSidePosMR[i][j]->SetRotation(m_rotation);
		}
	}
	//デバック用ここまで

}
