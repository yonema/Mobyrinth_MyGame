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


	return true;
}

