#include "stdafx.h"
#include "Player.h"
#include "LightManager.h"
#include "LevelObjectManager.h"


bool Player::Start()
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
	m_onWayPosition = m_position;
	m_mobius = FindGO<Mobius>("Mobius");


	//デバック用
	//後で消す
	m_dbgModel = NewGO<CModelRender>(0);
	m_dbgModel->Init("Assets/modelData/yuka.tkm");
	m_dbgModel2 = NewGO<CModelRender>(0);
	m_dbgModel2->Init("Assets/modelData/yuka.tkm");
	m_dbgModel3 = NewGO<CModelRender>(0);
	m_dbgModel3->Init("Assets/modelData/yuka.tkm");


	Init();

	SInitOBBData initData;
	initData.width = 150.0f;
	initData.length = 200.0f;
	initData.height = 300.0f;
	initData.position = m_position;
	initData.rotation = m_rotation;
	initData.pivot = { 0.5f,0.0f,0.5f };
	m_obb.Init(initData);

	initData.width = 100.0f;
	initData.length = 300.0f;
	initData.height = 400.0f;
	initData.position = { 0.0f,1800.0f,0.0f };
	initData.rotation = g_quatIdentity;
	initData.pivot = { 1.0f,0.75f,0.25f };
	m_obb2.Init(initData);

	Vector3* vertPos = m_obb.GetBoxVertex();
	Vector3* vertPos2 = m_obb2.GetBoxVertex();
	for (int i = 0; i < m_obbNum; i++)
	{
		m_dbgObbModel[i] = NewGO<CModelRender>(0);
		m_dbgObbModel[i]->Init("Assets/modelData/yuka.tkm");
		m_dbgObbModel[i]->SetPosition(vertPos[i]);
		m_dbgObbModel2[i] = NewGO<CModelRender>(0);
		m_dbgObbModel2[i]->Init("Assets/modelData/yuka.tkm");
		m_dbgObbModel2[i]->SetPosition(vertPos2[i]);

	}


	m_dbgObbCenter = NewGO<CModelRender>(0);
	m_dbgObbCenter->Init("Assets/modelData/yuka.tkm");
	m_dbgObbCenter->SetPosition({ 0.0f, 1800.0f, 0.0f });

	return true;
}

Player::~Player()
{
	DeleteGO(m_modelRender);

	//デバック用
	//後で消す
	DeleteGO(m_dbgModel);
	DeleteGO(m_dbgModel2);
	DeleteGO(m_dbgModel3);
	for (int i = 0; i < m_obbNum; i++)
	{
		DeleteGO(m_dbgObbModel[i]);
		DeleteGO(m_dbgObbModel2[i]);
	}
	DeleteGO(m_dbgObbCenter);

}

void Player::Init()
{
	//ゲームパッドの左スティックのX軸の入力情報を取得
	m_padLStickXF = 0.0f;

	//ウェイポイントの更新処理
	CheckWayPoint();
	//移動処理
	Move();
	//モデルの回転処理
	Rotation();

	m_onWayPosition += m_moveSpeed * 1.0 / 60.0f;
	GetOnStage();

	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
}

void Player::CheckWayPoint()
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

	Vector3 LpToRpVec = (*m_wayPointPos)[m_rpIndex] - (*m_wayPointPos)[m_lpIndex];
	LpToRpVec.Normalize();
	Vector3 LpToPlayerVec = m_position - (*m_wayPointPos)[m_lpIndex];
	LpToPlayerVec.Normalize();
	Vector3 RpToPlayerVec = m_position - (*m_wayPointPos)[m_rpIndex];
	RpToPlayerVec.Normalize();

	float LpDotPlayer = Dot(LpToRpVec, LpToPlayerVec);
	float RpDotPlayer = Dot(LpToRpVec, RpToPlayerVec);
	m_dbgDot1 = LpDotPlayer;
	m_dbgDot2 = RpDotPlayer;


	//左右のウェイポイントとの距離を調べる
	float f = 0.35f;
	if (LpDotPlayer > f && RpDotPlayer < -f)
	{
		//今のウェイポイントの間にいる
	}
	else if (LpDotPlayer <= f && m_padLStickXF < 0.0f)
	{
		//今のウェイポイントの間から、左側に出ていった
		m_wayPointState += 1;
		if (m_wayPointState > m_maxWayPointState)
		{
			//m_wayPointStateがMAXより大きかったら
			//一周したということだから、スタートの0にする
			m_wayPointState = 0;
		}
	}
	else if (RpDotPlayer >= -f && m_padLStickXF > 0.0f)
	{
		//今のウェイポイントの間から、右側から出ていった
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

void Player::Move()
{
	//移動する向きは毎フレーム計算した方がいいのかな？
	//それとも、m_wayPointStateの切り替の時にした方がいいのかな？
	//いや、今のやり方だと毎フレームやらなくてはいけない気がする

	//1.左右への移動する方向を計算する。

	//左へ移動する方向
	Vector3 moveToLeft = (*m_wayPointPos)[m_lpIndex] - m_onWayPosition;
	moveToLeft.Normalize();
	//右へ移動する方向
	Vector3 moveToRight = (*m_wayPointPos)[m_rpIndex] - m_onWayPosition;
	moveToRight.Normalize();


	//2.移動処理

	//とりあえずの処理
	//重力や、加速度、抵抗を実装するときは別のやり方で
	m_moveSpeed = g_vec3Zero;

	//移動する長さ
	float moveLen = 1000.0f;

	//ゲームパッドのR1ボタンの入力情報を取得(ダッシュ状態)
	if (g_pad[0]->IsPress(enButtonRB1) == true) {
		moveLen = 3000.0f;
	}


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

void Player::GetOnStage()
{
	m_upVec = g_vec3AxisY;
	m_finalWPRot.Apply(m_upVec);
	m_upVec.Scale(150.0f);
	if (m_mobius)
	{
		if (m_mobius->GetModel()->InIntersectLine(m_onWayPosition + m_upVec, m_onWayPosition - m_upVec))
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
	auto hitPos = m_mobius->GetModel()->GetIntersectPos();
	m_dbgModel->SetPosition(m_onWayPosition + m_upVec);
	m_dbgModel2->SetPosition(m_onWayPosition - m_upVec);
	m_dbgModel3->SetPosition(hitPos);
	

	//m_modelRender->SetPosition(hitPos);
	return;

}

void Player::Rotation()
{
	//左のウェイポイントから右のウェイポイントへのベクトル
	Vector3 lpToRpLen = (*m_wayPointPos)[m_rpIndex] - (*m_wayPointPos)[m_lpIndex];

	//左のウェイポイントからプレイヤーへのベクトル
	Vector3 lpToPlayerLen = m_onWayPosition - (*m_wayPointPos)[m_lpIndex];

	//補完率
	float ComplementRate = lpToPlayerLen.Length() / lpToRpLen.Length();

	//球面線形補完
	m_finalWPRot.Slerp(ComplementRate, (*m_wayPointRot)[m_lpIndex], (*m_wayPointRot)[m_rpIndex]);

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

void Player::Update()
{
	if (m_operationFlag == false) {
		return;
	}


	//ゲームパッドの左スティックのX軸の入力情報を取得
	m_padLStickXF = g_pad[0]->GetLStickXF();
	//左右の向きを設定
	if (m_padLStickXF < 0.0f)
		m_leftOrRight = enLeft;		//左向き
	else if (m_padLStickXF > 0.0f)
		m_leftOrRight = enRight;	//右向き


	//�f�o�b�N�p
	//��ŏ��
	m_obb.SetRotation(m_finalWPRot);
	m_obb.SetPosition(m_position);
	Vector3* boxVertex = m_obb.GetBoxVertex();
	for (int i = 0; i < m_obbNum; i++)
	{
		m_dbgObbModel[i]->SetPosition(boxVertex[i]);
	}

	m_obb2.SetPosition({ 0.0f,1800.0f,0.0f });
	Vector3* boxVertex2 = m_obb2.GetBoxVertex();
	for (int i = 0; i < m_obbNum; i++)
	{
		m_dbgObbModel2[i]->SetPosition(boxVertex2[i]);
	}
	m_dbgObbCenter->SetPosition({ 0.0f,1800.0f,0.0f });


	m_dbgOBBHit = CollisionOBBs(m_obb, m_obb2);


	//�����܂ŁA�f�o�b�N�p




	//ウェイポイントの更新処理
	CheckWayPoint();
	//移動処理
	Move();
	//モデルの回転処理
	Rotation();

	m_onWayPosition += m_moveSpeed * 1.0 / 60.0f;
	GetOnStage();

	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);

}



void Player::SetWayPointPos
(const std::size_t vecSize, std::vector<Vector3>*const posMap)
{
	//vectorのサイズの確保
	//m_wayPointPos->resize(vecSize);
	//ウェイポイントステートの最大の値を設定
	m_maxWayPointState = vecSize - 1;
	//m_wayPointPosにウェイポイントの「場所」を格納する
	m_wayPointPos = posMap;
	//std::vector<Vector3>::iterator it = posMap->begin();
	//for (int index = 0; it != posMap->end(); index++, it++)
	//{
	//	m_wayPointPos[index] = &it;
	//}
}
void Player::SetWayPointRot
(const std::size_t vecSize, std::vector<Quaternion>* rotMap)
{
	//vectorのサイズの確保
	//m_wayPointRot->resize(vecSize);
	//ウェイポイントステートの最大の値を設定
	m_maxWayPointState = vecSize - 1;
	//m_wayPointRotにウェイポイントの「回転」を格納する
	m_wayPointRot = rotMap;
	//std::map<int, Quaternion>::iterator it = rotMap->begin();
	//for (int index = 0; it != rotMap->end(); index++, it++)
	//{
	//	m_wayPointRot[index] = &it->second;
	//}
}


void Player::PostRender(RenderContext& rc)
{
	//テキスト用意
	wchar_t text[256];




	//描画開始
	m_font.Begin(rc);


	//�E�F�C�|�C���g�̃X�e�[�g
	swprintf(text, L"wayPointState:%02d", m_wayPointState);
	//描画
	m_font.Draw(text,
		{ -600.0f, 300.0f },
		{ 0.0f,0.0f,0.0f,1.0f },
		0.0f,
		1.0f,
		{ 0.0f,0.0f }
	);

	//�v���C���[�̍��E�̃E�F�C�|�C���g
	//���̃E�F�C�|�C���g
	swprintf(text, L"[%02d]", m_lpIndex);
	m_font.Draw(text,
		{ -110.0f, 50.0f },
		{ 1.0f,0.0f,0.0f,1.0f },
		0.0f,
		1.0f,
		{ 0.0f,0.0f }
	);
	//�E�̃E�F�C�|�C���g
	swprintf(text, L"[%02d]", m_rpIndex);
	m_font.Draw(text,
		{ 10.0f,50.0f },
		{ 1.0f,0.0f,0.0f,1.0f },
		0.0f,
		1.0f,
		{ 0.0f,0.0f }
	);

	//�X�e�[�W�Ƃ̓����蔻��
	swprintf(text, L"Hit%d", m_dbgHit);
	m_font.Draw(text,
		{ 110.0f, 150.0f },
		{ 1.0f,0.0f,0.0f,1.0f },
		0.0f,
		1.0f,
		{ 0.0f,0.0f }
	);
	//�X�e�[�W�ɗ����߂̕⊮��
	swprintf(text, L"rate%05f", m_mobius->GetModel()->getDbg());
	m_font.Draw(text,
		{ 110.0f, 120.0f },
		{ 1.0f,0.0f,0.0f,1.0f },
		0.0f,
		1.0f,
		{ 0.0f,0.0f }
	);


	//�E�F�C�|�C���g�؂�ւ��̂��߂̓��
	swprintf(text, L"����%02.2f", m_dbgDot1);
	m_font.Draw(text,
		{ -310.0f, 150.0f },
		{ 1.0f,0.0f,0.0f,1.0f },
		0.0f,
		1.0f,
		{ 0.0f,0.0f }
	);
	swprintf(text, L"右側%02.2f", m_dbgDot2);
	m_font.Draw(text,
		{ -310.0f, 120.0f },
		{ 1.0f,0.0f,0.0f,1.0f },
		0.0f,
		1.0f,
		{ 0.0f,0.0f }
	);


	//OBB�̓����蔻��
	swprintf(text, L"OBBHit%d", m_dbgOBBHit);
	m_font.Draw(text,
		{ -100.0f, 200.0f },
		{ 1.0f,0.0f,0.0f,1.0f },
		0.0f,
		1.0f,
		{ 0.0f,0.0f }
	);

	//描画終了

	m_font.End(rc);
}
