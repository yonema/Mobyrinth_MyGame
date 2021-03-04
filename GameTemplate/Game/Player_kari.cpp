#include "stdafx.h"
#include "Player_kari.h"
#include "LightManager.h"



bool Player_kari::Start()
{
	m_animationClips[enAnimClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClips[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimClip_Run].Load("Assets/animData/walk.tka");
	m_animationClips[enAnimClip_Run].SetLoopFlag(true);
	m_modelRender = NewGO<CModelRender>(0);
	m_modelRender->Init
	("Assets/modelData/unityChan.tkm",m_animationClips,enAnimClip_Num, enModelUpAxisY);
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);

	m_mobius = FindGO<Mobius>("Mobius");


	m_dbgModel = NewGO<CModelRender>(0);
	m_dbgModel->Init("Assets/modelData/yuka.tkm");
	m_dbgModel2 = NewGO<CModelRender>(0);
	m_dbgModel2->Init("Assets/modelData/yuka.tkm");

	return true;
}

Player_kari::~Player_kari()
{
	DeleteGO(m_modelRender);
}


void Player_kari::CheckWayPoint()
{
	///
	///
	/// m_wayPointStateは左周りで0から増えていく。
	/// m_wayPointStateが0の時、m_lpIndexは0,m_rpIndexは1になる。
	/// つまり、m_lpIndexは常にm_wayPointStateと同じ値になり、
	/// m_rpIndexはm_wayPointStateに1加算した値になる。
	/// そしてm_maxWayPointStateはm_wayPointStateの最大数を表す。
	/// m_maxWayPointStateが31だったらm_wayPointStateは31まで存在する。
	

	//1.プレイヤー自身の左右のウェイポイントを設定する
	//m_wayPointStateをもとにウェイポイントを設定する。
	m_rpIndex = m_wayPointState;	//右のウェイポイントはとm_wayPointStateは同じ値
	m_lpIndex = m_rpIndex + 1;		//左のウェイポイントは右のウェイポイントの1つ上の値
	if (m_lpIndex > m_maxWayPointState)
	{
		//左のウェイポイントがMAXより大きかったら
		//一周したということだから、スタートの0にする
		m_lpIndex = 0;
	}


	//2.m_wayPointStateの更新。
	//左右のウェイポイントとの距離で更新する。

	//ウェイポイントとの当たり判定の距離
	const float hitLen = 10.0f;

	//左のウェイポイントへのベクトル
	Vector3 toLpVec = m_wayPointPos[m_lpIndex] - m_position;

	//右のウェイポイントへのベクトル
	Vector3 toRpVec = m_wayPointPos[m_rpIndex] - m_position;

	//左右のウェイポイントとの距離を調べる
	if (toLpVec.Length() <= hitLen && m_padLStickXF < 0.0f)
	{
		//左のウェイポイントとの距離が当たり判定以下、かつ、
		//左に移動の入力があったら、
		//m_wayPointStateを加算して左に進める。
		m_wayPointState += 1;
		if (m_wayPointState > m_maxWayPointState)
		{
			//m_wayPointStateがMAXより大きかったら
			//一周したということだから、スタートの0にする
			m_wayPointState = 0;
		}

	}
	else if (toRpVec.Length() <= hitLen && m_padLStickXF > 0.0f)
	{
		//右のウェイポイントとの距離が当たり判定以下、かつ、
		//右に移動の入力があったら、
		//m_wayPointStateを減算して右に進める。
		m_wayPointState -= 1;
		if (m_wayPointState < 0)
		{
			//m_wayPointStateが0より小さかったら
			//一周したということだから、MAXの値にする
			m_wayPointState = m_maxWayPointState;
		}
	}

	return;

}

void Player_kari::Move()
{
	//移動する向きは毎フレーム計算した方がいいのかな？
	//それとも、m_wayPointStateの切り替の時にした方がいいのかな？
	//いや、今のやり方だと毎フレームやらなくてはいけない気がする

	//1.左右への移動する方向を計算する。

	//左へ移動する方向
	Vector3 moveToLeft = m_wayPointPos[m_lpIndex] - m_position;
	moveToLeft.Normalize();
	//右へ移動する方向
	Vector3 moveToRight = m_wayPointPos[m_rpIndex] - m_position;
	moveToRight.Normalize();


	//2.移動処理

	//とりあえずの処理
	//重力や、加速度、抵抗を実装するときは別のやり方で
	m_moveSpeed = g_vec3Zero;

	//移動する長さ
	const float moveLen = 500.0f;

	if (m_padLStickXF < 0.0f)
	{
		//左への移動の入力があったら
		//左への移動の計算する
		m_moveSpeed += moveToLeft * m_padLStickXF * -moveLen;
	}
	else if (m_padLStickXF > 0.0f)
	{
		//右への移動の入力があったら
		//右への移動の計算をする
		m_moveSpeed += moveToRight * m_padLStickXF * moveLen;
	}


	return;
}

void Player_kari::GetOnStage()
{
	Vector3 addPos = g_vec3AxisY;
	m_finalWPRot.Apply(addPos);
	addPos.Scale(-200.0f);

	if (m_mobius)
	{
		if (m_mobius->GetModel()->InIntersectLine(m_position, m_position + addPos))
		{
			m_position = m_mobius->GetModel()->GetIntersectPos();
			m_dbgHit = true;
		}
		else
			m_dbgHit = false;
	}
	else
	{
		m_mobius = FindGO<Mobius>("Mobius");
	}
	m_dbgModel2->SetPosition(m_mobius->GetModel()->GetIntersectPos());

	m_dbgModel->SetPosition(m_position + addPos);

	return;

}

void Player_kari::Rotation()
{
	//左のウェイポイントから右のウェイポイントへのベクトル
	Vector3 lpToRpLen = m_wayPointPos[m_rpIndex] - m_wayPointPos[m_lpIndex];

	//左のウェイポイントからプレイヤーへのベクトル
	Vector3 lpToPlayerLen = m_position - m_wayPointPos[m_lpIndex];

	//補完率
	float ComplementRate = lpToPlayerLen.Length() / lpToRpLen.Length();

	//球面線形補完
	m_finalWPRot.Slerp(ComplementRate, m_wayPointRot[m_lpIndex], m_wayPointRot[m_rpIndex]);

	//キャラクターの左右の向きに合わせて回転
	if (m_leftOrRight == enLeft)
	{
		m_rotation.SetRotationDegY(90.0f);
	}
	else if (m_leftOrRight == enRight)
	{
		m_rotation.SetRotationDegY(-90.0f);
	}
	m_rotation.Multiply(m_finalWPRot);

	return;
}

void Player_kari::Update()
{
	//ゲームパッドの左スティックのX軸の入力情報を取得
	m_padLStickXF = g_pad[0]->GetLStickXF();
	//左右の向きを設定
	if (m_padLStickXF < 0.0f)
		m_leftOrRight = enLeft;		//左向き
	else if (m_padLStickXF > 0.0f)
		m_leftOrRight = enRight;	//右向き

	//ウェイポイントの更新処理
	CheckWayPoint();
	//移動処理
	Move();
	//モデルの回転処理
	Rotation();

	m_position += m_moveSpeed * 1.0 / 60.0f;
	GetOnStage();

	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
}

void Player_kari::InitWayPointPos(const std::size_t vecSize, std::map<int, Vector3>& posMap)
{
	//vectorのサイズの確保
	m_wayPointPos.resize(vecSize);
	//ウェイポイントステートの最大の値を設定
	m_maxWayPointState = vecSize - 1;
	//m_wayPointPosにウェイポイントの「場所」を格納する
	std::map<int, Vector3>::iterator it = posMap.begin();
	for (int index = 0; it != posMap.end(); index++, it++)
	{
		m_wayPointPos[index] = it->second;
	}
}
void Player_kari::InitWayPointRot(const std::size_t vecSize, std::map<int, Quaternion>& rotMap)
{
	//vectorのサイズの確保
	m_wayPointRot.resize(vecSize);
	//ウェイポイントステートの最大の値を設定
	m_maxWayPointState = vecSize - 1;
	//m_wayPointRotにウェイポイントの「回転」を格納する
	std::map<int, Quaternion>::iterator it = rotMap.begin();
	for (int index = 0; it != rotMap.end(); index++, it++)
	{
		m_wayPointRot[index] = it->second;
	}
}

void Player_kari::PostRender(RenderContext& rc)
{
	//テキスト用意
	wchar_t text[256];




	//描画開始
	m_font.Begin(rc);

	swprintf(text, L"wayPointState:%02d", m_wayPointState);
	//描画
	m_font.Draw(text,
		{ -600.0f, 300.0f },
		{ 0.0f,0.0f,0.0f,1.0f },
		0.0f,
		1.0f,
		{ 0.0f,0.0f }
	);
	swprintf(text, L"[%02d]", m_rpIndex);
	m_font.Draw(text,
		{ 10.0f,50.0f },
		{ 1.0f,0.0f,0.0f,1.0f },
		0.0f,
		1.0f,
		{ 0.0f,0.0f }
	);
	swprintf(text, L"[%02d]", m_lpIndex);
	m_font.Draw(text,
		{ -110.0f, 50.0f },
		{ 1.0f,0.0f,0.0f,1.0f },
		0.0f,
		1.0f,
		{ 0.0f,0.0f }
	);

	swprintf(text, L"Hit%d", m_dbgHit);
	m_font.Draw(text,
		{ 110.0f, 150.0f },
		{ 1.0f,0.0f,0.0f,1.0f },
		0.0f,
		1.0f,
		{ 0.0f,0.0f }
	);




	//描画終了
	m_font.End(rc);
}
