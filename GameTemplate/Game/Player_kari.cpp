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

	m_charaCon.Init(50.0f, 10.0f, m_position);

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
	/// m_wayPointStateが0の時、m_lpIndexは0,m_rpIndexは1になる。
	/// つまり、m_lpIndexは常にm_wayPointStateと同じ値になり、
	/// m_rpIndexはm_wayPointStateに1加算した値になる。
	/// そしてm_maxWayPointStateはm_wayPointStateの最大数を表す。
	/// m_maxWayPointStateが31だったらm_wayPointStateは31まで存在する。
	

	//プレイヤー自身の左右のウェイポイントを設定する
	m_lpIndex = m_wayPointState;
	m_rpIndex = m_lpIndex + 1;
	if (m_rpIndex > m_maxWayPointState)
	{
		m_rpIndex = 0;
	}

	const float hitLen = 10.0f;

	//左のウェイポイントへのベクトル
	Vector3 toLpVec = m_wayPointPos[m_lpIndex] - m_position;
	//右のウェイポイントへのベクトル
	Vector3 toRpVec = m_wayPointPos[m_rpIndex] - m_position;

	//左右のウェイポイントとの距離を調べる
	if (toLpVec.Length() <= hitLen && g_pad[0]->GetLStickXF() > 0.0f)
	{
		m_wayPointState -= 1;
		if (m_wayPointState < 0)
		{
			m_wayPointState = m_maxWayPointState;
		}
	}
	else if (toRpVec.Length() <= hitLen && g_pad[0]->GetLStickXF() < 0.0f)
	{
		m_wayPointState += 1;
		if (m_wayPointState > m_maxWayPointState)
		{
			m_wayPointState = 0;
		}
	}

	return;

}

void Player_kari::Move()
{
	//移動する向きは毎フレーム計算した方がいいのかな？
	//それとも、m_wayPointStateの切り替の時にした方がいいのかな？
	//いや、今のやり方だと毎フレームやらなくてはいけない気がする


	//左へ移動する方向を計算
	Vector3 moveToRight = m_wayPointPos[m_rpIndex] - m_position;
	moveToRight.Normalize();
	//右へ移動する方向を計算
	Vector3 moveToLeft = m_wayPointPos[m_lpIndex] - m_position;
	moveToLeft.Normalize();


	//とりあえずの処理
	//重力や、加速度、抵抗を実装するときは別のやり方で
	m_moveSpeed = g_vec3Zero;

	const float moveLen = 500.0f;
	if (g_pad[0]->GetLStickXF() > 0.0f)
	//左への移動の計算
	m_moveSpeed += moveToLeft * g_pad[0]->GetLStickXF() * moveLen;
	else if (g_pad[0]->GetLStickXF() < 0.0f)
	//左への移動の計算
	m_moveSpeed += moveToRight * g_pad[0]->GetLStickXF() * -moveLen;


	return;
}

void Player_kari::Rotation()
{
	return;
}

void Player_kari::Update()
{
	CheckWayPoint();
	Move();
	Rotation();

	m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
}

void Player_kari::InitWayPointPos(const std::size_t vecSize, std::map<int, Vector3>& posMap)
{
	//vectorのサイズの確保
	m_wayPointPos.resize(vecSize);
	//ウェイポイントの最大数をを格納しておく
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
	//ウェイポイントの最大数をを格納しておく
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

	swprintf(text, L"wayPointState:%02d", m_wayPointState);


	//描画開始
	m_font.Begin(rc);

	//描画
	m_font.Draw(text,
		{ -200.0f, 200.0f },
		{ 0.0f,0.0f,0.0f,1.0f },
		0.0f,
		1.0f,
		{ 0.5f,0.5f }
	);

	//描画終了
	m_font.End(rc);
}
