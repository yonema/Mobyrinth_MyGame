#include "stdafx.h"
#include "OOoneway.h"

//スタート関数
bool OOoneway::StartSub()
{
	//モデルの初期化とタイプの設定
	Init("Assets/modelData/oneway.tkm", enOneway);

	GetModelRender()->Deactivate();

	//通れる時のモデルの生成と初期化
	m_canPassMR = NewGO<CModelRender>(0);
	m_canPassMR->Init("Assets/modelData/oneway2.tkm", D3D12_CULL_MODE_NONE);
	m_canPassMR->SetPosition(m_position);
	m_canPassMR->SetRotation(m_rotation);
	m_canPassMR->SetScale(m_scale);
	m_canPassMR->Deactivate();

	
	//通れる時のモデルの生成と初期化
	m_rightCanPassMR = NewGO<CModelRender>(0);
	m_rightCanPassMR->Init("Assets/modelData/oneway2_r.tkm", D3D12_CULL_MODE_NONE);
	m_rightCanPassMR->SetPosition(m_position);
	m_rightCanPassMR->SetRotation(m_rotation);
	m_rightCanPassMR->SetScale(m_scale);
	m_rightCanPassMR->Deactivate();

	//通れる時のモデルの生成と初期化
	m_rightMR = NewGO<CModelRender>(0);
	m_rightMR->Init("Assets/modelData/oneway_r.tkm", D3D12_CULL_MODE_NONE);
	m_rightMR->SetPosition(m_position);
	m_rightMR->SetRotation(m_rotation);
	m_rightMR->SetScale(m_scale);
	m_rightMR->Deactivate();

	//左向きか？
	if (m_leftOrRight == enLeft)
	{
		//左向き

		//左向き（通常の向き）の通れないモデルを有効化
		GetModelRender()->Activate();
	}
	else
	{
		//右向き

		//右向き（反対向き）の通れないモデルを有効化
		m_rightMR->Activate();

	}
	

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

	DeleteGO(m_canPassMR);
	DeleteGO(m_rightCanPassMR);
	DeleteGO(m_rightMR);
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
	{
		//向いている時は
		//通れる
		m_sideOBB[m_leftOrRight].SetExceptionFlag(true);

		//左向きか？
		if (m_leftOrRight == enLeft)
		{
			//左向き

			//左向きの通れるモデルを有効化
			m_canPassMR->Activate();
			//右向きの通れるモデルを無効化
			m_rightCanPassMR->Deactivate();
		}
		else
		{
			//右向き

			//左向きの通れるモデルを無効化
			m_canPassMR->Deactivate();
			//右向きの通れるモデルを有効化
			m_rightCanPassMR->Activate();
		}

		//通れないモデルを無効化
		GetModelRender()->Deactivate();
		m_rightMR->Deactivate();

	}
	else
	{
		//向いていない時は
		//通れない
		m_sideOBB[m_leftOrRight].SetExceptionFlag(false);

		//左向きか？
		if (m_leftOrRight == enLeft)
		{
			//左向き

			//左向きの通れないモデルを有効化
			GetModelRender()->Activate();
			//右向きの通れないモデルを無効化
			m_rightMR->Deactivate();
		}
		else
		{
			//右向き

			//左向きの通れないモデルを無効化
			GetModelRender()->Deactivate();
			//右向きの通れないモデルを有効化
			m_rightMR->Activate();
		}

		//通れるモデルを無効化
		m_canPassMR->Deactivate();
		m_rightCanPassMR->Deactivate();
	}


	if (m_canPassMR->IsActive())
	{
		m_canPassMR->SetPosition(m_position);
		m_canPassMR->SetRotation(m_rotation);
		m_canPassMR->SetScale(m_scale);
	}
	if (m_rightCanPassMR->IsActive())
	{
		m_rightCanPassMR->SetPosition(m_position);
		m_rightCanPassMR->SetRotation(m_rotation);
		m_rightCanPassMR->SetScale(m_scale);
	}
	if (m_rightMR->IsActive())
	{
		m_rightMR->SetPosition(m_position);
		m_rightMR->SetRotation(m_rotation);
		m_rightMR->SetScale(m_scale);
	}
}