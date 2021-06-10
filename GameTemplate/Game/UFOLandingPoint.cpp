#include "stdafx.h"
#include "UFOLandingPoint.h"

//スタート関数
bool CUFOLandingPoint::StartSub()
{
	//モデルの初期化とタイプの設定
	Init("Assets/modelData/UFO_Landing.tkm", enUFOLandingPoint);

	const float scale = 0.4f;
	m_scale = { scale ,1.0f ,scale };

	//OBBWorldに自身のOBBの登録を消去させる
	COBBWorld::GetInstance()->RemoveOBB(&GetOBB());

	//自己発光色を設定
	//SetEmissionColor({ 0.5f,0.5f,0.0f,1.0f });

	//OBBのパラメーターを設定する
	GetOBB().SetDirectionLength({ 2000.0f,750.0f,500.0f });
	GetOBB().SetPivot({ 0.5f,0.0f,0.5f });

	//端のOBBのパラメーターの設定をする
	for (int i = 0; i < enLeftAndRightNum; i++)
	{
		m_sideOBB[i].SetDirectionLength({ 10.0f,750.0f,500.0f });
		m_sideOBB[i].SetPivot({ 0.5f,0.0f,0.5f });
		m_sideOBB[i].SetPosition(m_position);
		m_sideOBB[i].SetRotation(m_rotation);
	}




	//デバック用
#ifdef MY_DEBUG
	//後で消す
	Vector3* vertPos = GetOBB().GetBoxVertex();
	for (int i = 0; i < m_vertNum; i++)
	{
		//OBBの頂点を見るためのモデルの生成と初期化
		m_dbgVertPosMR[i] = NewGO<CModelRender>(0);
		m_dbgVertPosMR[i]->Init("Assets/modelData/dbgBox.tkm");
		m_dbgVertPosMR[i]->SetPosition(vertPos[i]);
		m_dbgVertPosMR[i]->SetRotation(m_rotation);
	}

	for (int i = 0; i < enLeftAndRightNum; i++)
	{
		//端のOBBを見るためのモデルの生成と初期化
		vertPos = m_sideOBB[i].GetBoxVertex();
		for (int j = 0; j < m_vertNum; j++)
		{
			m_dbgSidePosMR[i][j] = NewGO<CModelRender>(0);
			m_dbgSidePosMR[i][j]->Init("Assets/modelData/dbgBox.tkm");
			m_dbgSidePosMR[i][j]->SetPosition(vertPos[j]);
			m_dbgSidePosMR[i][j]->SetRotation(m_rotation);
		}
	}
#endif
	//デバック用ここまで

	//端のOBBのアップデート
	UpdateSideOBB();

	return true;
}

//デストラクタ
CUFOLandingPoint::~CUFOLandingPoint()
{
	//デバック用
#ifdef MY_DEBUG
	//後で消す
	for (int i = 0; i < m_vertNum; i++)
	{
		//OBBの頂点を見るためのモデルの破棄
		DeleteGO(m_dbgVertPosMR[i]);
	}

	for (int i = 0; i < enLeftAndRightNum; i++)
	{
		//端のOBBを見るためのモデルの破棄
		for (int j = 0; j < m_vertNum; j++)
		{
			DeleteGO(m_dbgSidePosMR[i][j]);
		}
	}
#endif
	//デバック用ここまで
}

/// <summary>
/// 端のOBBのアップデート
/// </summary>
void CUFOLandingPoint::UpdateSideOBB()
{
	//横幅
	const float sideLen = 10.0f;
	//左右へのベクトル
	Vector3 sideVec[enLeftAndRightNum] = { g_VEC3_LEFT, g_VEC3_RIGHT };

	//左と右の二回分繰り返す
	for (int i = 0; i < enLeftAndRightNum; i++)
	{
		//左右のベクトルを自身の回転で回す
		m_rotation.Apply(sideVec[i]);
		//横幅の大きさを掛ける
		sideVec[i].Scale(sideLen);
		//なんか逆になってるから、反転させる
		sideVec[i].Scale(-1.0f);

		//OBBの更新
		m_sideOBB[i].SetPosition(m_position + sideVec[i]);
		m_sideOBB[i].SetRotation(m_rotation);
	}
#ifdef MY_DEBUG
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
#endif
	//デバック用ここまで

}
