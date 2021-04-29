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

	//UFOの着地点の生成と初期化
	m_ufoLandingPoint = NewGO<CUFOLandingPoint>(0);
	m_ufoLandingPoint->SetPosition(m_position);


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

	//UFOの着地点の破棄
	DeleteGO(m_ufoLandingPoint);

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
	m_upVec = g_vec3Up;
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
	case enLanding:
		Landing();
		break;
	case enLeave:
		Leave();
		break;
	}

	//ライトの更新
	Vector3 upVec = m_upVec;
	upVec.Scale(300.0f);
	m_spotLight->SetPosition(m_position + upVec);
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
	const float switchingTime = 1.5f;

	//切り替え時間になったら
	if (m_timer >= switchingTime)
	{
		//フラグを切り合える
		m_searchFlag = !m_searchFlag;
		//タイマーを0に戻す
		m_timer = 0.0f;
	}

	
	if (m_searchFlag)
	{
		//捜索中なら
		m_modelRender->SetEmissionColor({ 0.5f,0.5f,0.0f,1.0f });
		m_spotLight->SetColor({ 300.0f,300.0f,0.0f,1.0f });
		//プレイヤーを衝突しているか？
		if (IsHitPlayer())
		{
			//プレイヤーを見つけた
			m_modelRender->SetEmissionColor({ 1.0f,0.0f,0.0f,1.0f });
			m_spotLight->SetColor({ 600.0f,0.0f,0.0f,1.0f });
			m_updateState = enCapture;
			m_pPlayer->SetCapturedUFOFlag(true);
			m_pPlayer->SetRotation(m_rotation);
			if (m_pPlayer->GetHoldObject())
				m_pPlayer->GetReversibleObject()->StateToCancel();
			m_moveSpeed = 0.0f;
			m_timer = 0.0f;

		}
	}
	else
	{
		//捜索中ではない
		m_modelRender->SetEmissionColor({ 0.0f,0.0f,0.0f,1.0f });
		m_spotLight->SetColor({ 0.0f,0.0f,0.0f,1.0f });

	}
}

//プレイヤーを見つけた時の処理
void CUFO::Capture()
{
	//アップベクトルを得る
	Vector3 upVec = m_upVec;
	//イイ感じに伸ばす
	const float upVecLen = 500.0f;
	upVec.Scale(upVecLen);

	//UFOに捕まる位置
	Vector3 capturePos = m_position + upVec;

	//プレイヤーに加えるベクトル
	Vector3 addVec = capturePos - m_pPlayer->GetPosition();
	//デルタタイムを掛けておく
	addVec.Scale(GameTime().GetFrameDeltaTime());
	//プレイヤーの座標を設定
	m_pPlayer->SetPosition(m_pPlayer->GetPosition() + addVec);
	m_pPlayer->SetRotation(m_rotation);
	//タイマーの切り替え時間
	const float switchingTimer = 2.0f;
	if (m_timer >= 2.0f)
	{
		//タイマーが切り替え時間になったら
		//タイマーを初期化して
		m_timer = 0.0f;
		//プレイヤーを運ぶ処理へ
		m_updateState = enTransport;
	}
	else
		//まだならタイマーを進める
		m_timer += GameTime().GetFrameDeltaTime();

}

//プレイヤーを運ぶ処理
void CUFO::Transport()
{
	//タイマーが0.0fの時に
	//つまり最初の一回だけ呼ばれる
	if (m_timer == 0.0f)
	{
		//ウェイポイントの最大値
		const int maxWayPoint = 31;

		//現在の左側のウェイポイントの反対側のウェイポイントを出す。
		//0を含むため最大値に1加算してから、反対側を出すため半分にして、
		//現在の左側のウェイポイントにその値を加算している。
		int reverseLp = GetLeftWayPointIndex() + (maxWayPoint + 1) / 2;

		//右側から回って行くのか左側から行くのか決める
		if (GetLeftWayPointIndex() == m_ufoLandingPoint->GetLeftWayPointIndex())
		{
			//自身の左側のウェイポイントと、着地点の左側のウェイポイントが同じなら

			//右の単位方向ベクトル
			Vector3 rightVec = g_vec3Right;
			//自身の回転を掛ける
			m_rotation.Apply(rightVec);
			//自身（UFO）から着地点へのベクトル
			Vector3 UFOToLandPointVec = m_ufoLandingPoint->GetPosition() - m_position;
			//正規化する
			UFOToLandPointVec.Normalize();
			//着地点への
			float landVecDotRightVec = Dot(UFOToLandPointVec, rightVec);
			if (landVecDotRightVec >= 0.0f)
				m_leftOrRight = enLeft;
			else
				m_leftOrRight = enRight;
		}
		else if (reverseLp > 31)
		{
			reverseLp -= 32;
			if (GetLeftWayPointIndex() < m_ufoLandingPoint->GetLeftWayPointIndex() ||
				m_ufoLandingPoint->GetLeftWayPointIndex() <= reverseLp)
				m_leftOrRight = enLeft;
			else
				m_leftOrRight = enRight;
		}
		else
		{
			if (m_ufoLandingPoint->GetLeftWayPointIndex() <= reverseLp)
			{
				m_leftOrRight = enLeft;
			}
			else
			{
				m_leftOrRight = enRight;
			}
		}


		m_moveSpeed = m_defaultSpeed;
	}

	//アップベクトルを得る
	Vector3 upVec = m_upVec;
	//イイ感じに伸ばす
	const float upVecLen = 500.0f;
	upVec.Scale(upVecLen);

	//UFOに捕まる位置
	Vector3 capturePos = m_position + upVec;

	if (IsHitObject(*this, *m_ufoLandingPoint))
	{
		m_updateState = enLanding;
		m_timer = 0.0f;
	}
	else
		m_timer += GameTime().GetFrameDeltaTime();

	m_pPlayer->SetPosition(capturePos);
	m_pPlayer->SetRotation(m_rotation);
	m_pPlayer->SetLeftPointIndex(GetLeftWayPointIndex());
	m_pPlayer->SetRightPointIndex(GetRightWayPointIndex());
}

void CUFO::Landing()
{
	//m_ufoLandingPoint->UpdateSideOBB();
	//m_timer += GameTime().GetFrameDeltaTime();
	const float goDownTimer = 1.0f;
	const float goUpTimer = goDownTimer * 2.0f;

	//アップベクトルを得る
	Vector3 upVec = m_upVec;
	//イイ感じに伸ばす
	const float upVecLen = 500.0f;
	upVec.Scale(upVecLen);
	//UFOに捕まる位置
	Vector3 capturePos = m_position + upVec;

	Vector3 dist = (m_ufoLandingPoint->GetPosition() + upVec);
	dist -= capturePos;
	float distLen = dist.Length();
	const float maxDistLen = 1000.0f;
	const float minDistlen = 100.0f;
	Vector3 landUpVec = g_vec3Up;
	m_ufoLandingPoint->GetRotation().Apply(landUpVec);
	float upDotLandUp = Dot(m_upVec, landUpVec);
	float threshold = 0.0001f;

	if (!CollisionOBBs(GetOBB(), m_ufoLandingPoint->GetSideOBB(m_leftOrRight)) && 
		m_timer == 0.0f)
	{
		m_moveSpeed = m_defaultSpeed;
		m_moveSpeed *= distLen / maxDistLen;
	}
	else 
	{
		m_timer += GameTime().GetFrameDeltaTime();
		const float vecScale = 400.0f;
		if (m_timer <= goDownTimer)
		{
			m_getOnStageFlag = false;
			m_moveSpeed = 0.0f;
			Vector3 goDownVec = m_upVec;
			goDownVec.Scale(-vecScale);
			goDownVec.Scale(GameTime().GetFrameDeltaTime());
			m_position += goDownVec;
		}
		else if (m_timer <= goUpTimer)
		{
			Vector3 goUpVec = m_upVec;
			goUpVec.Scale(vecScale);
			goUpVec.Scale(GameTime().GetFrameDeltaTime());
			m_position += goUpVec;
		}
		else
		{
			m_updateState = enLeave;
		}
	}

	if (m_timer <= goDownTimer)
	{
		m_pPlayer->SetPosition(capturePos);
		m_pPlayer->SetRotation(m_rotation);
		m_pPlayer->SetWayPointState(GetLeftWayPointIndex());
		m_pPlayer->SetLeftPointIndex(GetLeftWayPointIndex());
		m_pPlayer->SetRightPointIndex(GetRightWayPointIndex());
	}
	else if (m_updateState == enLeave)
	{
		m_timer = 0.0f;

	}

}

void CUFO::Leave()
{
	m_leftOrRight = enRight;
	if (m_timer == 0.0f)
	{
		m_onWayPosition = m_position;

		m_getOnStageFlag = true;

		//ウェイポイントの最大数
		const int maxWayPointNum = 31;
		int nextIndex = m_ufoLandingPoint->GetLeftWayPointIndex();
		if (m_leftOrRight == enLeft)
		{
			//左に動いたら、特に何もいじらない
		}
		else
		{
			//右に動いたので、次のウェイポイントを加算する

			nextIndex--;
			//ウェイポイントの最大数より大きかったら
			if (nextIndex < 0)
				//一周して0にする
				nextIndex = maxWayPointNum;
		}
		//自身の左側のウェイポイントを更新する
		SetLeftWayPointIndex(nextIndex);
	}
	m_moveSpeed = m_defaultSpeed;
	const float switchingTime = 2.0f;
	if (m_timer >= switchingTime)
	{
		m_updateState = enSearch;
		m_timer = 0.0f;
		m_pPlayer->SetCapturedUFOFlag(false);
	}
	else
		m_timer += GameTime().GetFrameDeltaTime();

}

//移動処理
void CUFO::Move()
{
	//次のウェイポイント
	int nextIndex = GetRightWayPointIndex();
	//最終的な移動速度	//移動速度にデルタタイムを掛ける
	float finalMoveSpeed = m_moveSpeed * GameTime().GetFrameDeltaTime();
	//ウェイポイント上の次の座標を計算する
	m_onWayPosition = CLevelObjectManager::GetInstance()->CalcWayPointNextPos
	(GetRightWayPointIndex(), m_onWayPosition, finalMoveSpeed, m_leftOrRight, &nextIndex);

	//ウェイポイントの最大数
	const int maxWayPointNum = 31;
	if (m_leftOrRight == enLeft)
	{
		//左に動いたら、特に何もいじらない
	}
	else
	{
		//右に動いたので、次のウェイポイントを加算する

		nextIndex++;
		//ウェイポイントの最大数より大きかったら
		if (nextIndex > maxWayPointNum)
			//一周して0にする
			nextIndex = 0;
	}
	//自身の左側のウェイポイントを更新する
	SetLeftWayPointIndex(nextIndex);
}

//メビウスの輪のステージにイイ感じに合わせる処理
void CUFO::GetOnStage()
{
	if (!m_getOnStageFlag)
		return;
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