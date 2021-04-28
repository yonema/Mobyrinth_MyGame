#include "stdafx.h"
#include "UFO.h"

//スタート関数
bool CUFO::PureVirtualStart()
{
	//モデルレンダラーの生成と初期化
	m_modelRender = NewGO<CModelRender>(0);
	m_modelRender->Init("Assets/modelData/UFO.tkm");
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);

	//アップベクトルを現在の回転に合わせる
	m_rotation.Apply(m_upVec);
	//ウェイポイント上の座標を更新する
	m_onWayPosition = m_position;

	//OBBを調節する
	GetOBB().SetPivot({ 0.5f,0.0f,0.5f });
	GetOBB().SetDirectionLength({ 400.0f,200.0f,400.0f });

	//スポットライトの生成と初期化
	m_spotLight = NewGO<CSpotLight>(0);
	Vector3 up = m_upVec;
	up.Scale(100.0f);
	m_spotLight->SetPosition({ 0.0f,2000.0f, 0.0f });
	m_spotLight->SetColor({ 300.0f, 300.0f, 0.0f, 0.0f });
	m_spotLight->SetRange(500.0f);
	up.Normalize();
	up.Scale(-1.0f);
	m_spotLight->SetDirection(g_vec3Down);

	//CModelRender* model = NewGO<CModelRender>(0);
	//model->Init("Assets/modelData/fire.tkm");
	//model->SetPosition({ 0.0f,1600.0f,0.0f });
	//Quaternion qRot;
	//qRot.SetRotationDegX(-20.0f);
	//model->SetScale({ 3.0f,3.0f,3.0f });
	//model->SetRotation(qRot);

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
	for (int i = 0; i < 2; i++)
	{
		//レイの始点と終点を見るためのモデル
		m_dbgRay[i] = NewGO<CModelRender>(0);
		m_dbgRay[i]->Init("Assets/modelData/dbgBox.tkm");
	}
	//デバック用ここまで

	return true;
}

//デストラクタ
CUFO::~CUFO()
{
	//モデルレンダラーの破棄
	DeleteGO(m_modelRender);

	//スポットライトの破棄
	DeleteGO(m_spotLight);

	//デバック用
	//後で消す
	for (int i = 0; i < m_vertNum; i++)
	{
		DeleteGO(m_dbgVertPosMR[i]);
	}
	for (int i = 0; i < 2; i++)
	{
		DeleteGO(m_dbgRay[i]);
	}
	//デバック用ここまで
}

//アップデート関数
void CUFO::PureVirtualUpdate()
{
	//移動処理
	Move();
	//回転を現在の座標に合わせてた回転にする
	CheckRotation();
	//アップベクトルを回転に合わせる
	m_rotation.Apply(m_upVec);
	m_upVec.Normalize();
	//ステージの上に乗る
	GetOnStage();

	//アップデートのステートで行動を割り振る
	switch (m_updateState)
	{
	case enSearch:
		//プレイヤーを探す処理
		Search();
		break;
	case enCapture:
		//プレイヤーを見つけた時の処理
		Capture();
		break;
	case enTransport:
		//プレイヤーを運ぶ処理
		Transport();
		break;
	}

	//ライトの更新
	m_spotLight->SetPosition(m_position);
	Vector3 downVec = m_upVec;
	downVec.Scale(-1.0f);
	m_spotLight->SetDirection(downVec);

	//モデルレンダラーの更新
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);

	//デバック用
	//後で消す
	Vector3* vertPos = GetOBB().GetBoxVertex();
	for (int i = 0; i < m_vertNum; i++)
	{
		m_dbgVertPosMR[i]->SetPosition(vertPos[i]);
		m_dbgVertPosMR[i]->SetRotation(m_rotation);

	}
	//デバック用ここまで
}

//プレイヤーを探す処理
void CUFO::Search()
{
	//タイマーにデルタタイムを加算
	m_timer += GameTime().GetFrameDeltaTime();

	//捜索フラグの切り替えと時間
	const int switchingTime = 3.0f;

	//切り替え時間になったら
	if (m_timer >= switchingTime)
	{
		//フラグを切り合える
		m_searchFlag = !m_searchFlag;
		//タイマーを0に戻す
		m_timer = 0.0f;
	}

	//捜索中なら
	if (m_searchFlag)
	{
		m_modelRender->SetEmissionColor({ 0.5f,0.5f,0.0f,1.0f });
		m_spotLight->SetColor({ 300.0f,300.0f,0.0f,1.0f });
		//プレイヤーを衝突しているか？
		if (IsHitPlayer())
		{
			//プレイヤーを見つけた
			m_modelRender->SetEmissionColor({ 1.0f,0.0f,0.0f,1.0f });
			m_spotLight->SetColor({ 600.0f,0.0f,0.0f,1.0f });

		}
	}
	else
	{
		m_modelRender->SetEmissionColor({ 0.0f,0.0f,0.0f,1.0f });
		m_spotLight->SetColor({ 0.0f,0.0f,0.0f,1.0f });

	}
}

//プレイヤーを見つけた時の処理
void CUFO::Capture()
{

}

//プレイヤーを運ぶ処理
void CUFO::Transport()

{

}

//移動処理
void CUFO::Move()
{
	//右に動く
	bool right = true;
	//次のウェイポイント
	int nextIndex = GetRightWayPointIndex();
	//最終的な移動速度	//移動速度にデルタタイムを掛ける
	float finalMoveSpeed = m_moveSpeed * GameTime().GetFrameDeltaTime();
	//ウェイポイント上の次の座標を計算する
	m_onWayPosition = CLevelObjectManager::GetInstance()->CalcWayPointNextPos
	(GetRightWayPointIndex(), m_onWayPosition, finalMoveSpeed, right, &nextIndex);

	//右に動いたので、次のウェイポイントを加算する
	nextIndex++;
	//ウェイポイントの最大数
	const int maxWayPointNum = 31;
	//ウェイポイントの最大数より大きかったら
	if (nextIndex > maxWayPointNum)
		//一周して0にする
		nextIndex = 0;
	//自身の左側のウェイポイントを更新する
	SetLeftWayPointIndex(nextIndex);
}

//メビウスの輪のステージにイイ感じに合わせる処理
void CUFO::GetOnStage()
{
	//Upベクトルをイイ感じに伸ばす。
	//伸ばした先がレイの始点となる
	Vector3 upVec = g_vec3Up;
	m_rotation.Apply(upVec);
	upVec.Scale(500.0f);

	//デバック用
	//後で消す
	m_dbgRay[0]->SetPosition(m_onWayPosition + upVec);
	m_dbgRay[1]->SetPosition(m_onWayPosition - upVec);
	//デバック用ここまで
	if (m_mobius)
	{
		//メビウスの輪が見つかっていたら
		//メビウスの輪のモデルのポリゴンと、レイの当たり判定を取る
		if (m_mobius->GetModel()
			->InIntersectLine(m_onWayPosition + upVec, m_onWayPosition - upVec))
		{
			//ポリゴンとレイの交差点をを取ってきてモデルの座標に入れる
			m_position = m_mobius->GetModel()->GetIntersectPos();

		}
		else
		{
			m_position = m_onWayPosition;
		}
	}
	else
	{
		//メビウスの輪（ステージ）が見つかっていなかったら
		//探してreturnする
		m_mobius = FindGO<Mobius>("Mobius");
		return;
	}
}