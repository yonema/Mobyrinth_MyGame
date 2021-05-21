#include "stdafx.h"
#include "StartDirecting.h"

#include "GameCamera.h"
#include "Player.h"
#include "Pause.h"

bool StartDirecting::Start()
{
	m_position = { 0.0f,1740.0f,0.0f };
	m_onWayPosition = m_position;
	m_mobius = FindGO<Mobius>("Mobius");

	m_startPosition = m_position;

	m_player = FindGO<Player>("Player");
	m_gameCamera = FindGO<GameCamera>("GameCamera");


	return true;
}

StartDirecting::~StartDirecting()
{

}

void StartDirecting::Update()
{
	//カメラのターゲットを見えない形で動かして
	//カメラを動かす。
	
	//wayポイント[16]まで動かす。

	//演出をしない状態、またはワイプが終了していなかったら
	//何もせずにreturn
	//if (m_checkStartDirecting == false || !m_wipeEndFlag) {
	//	return;
	//}

	////ちょっと待ってから演出を開始する
	//if (m_startTimer < /*0.5f*/0.5f)
	//{
	//	m_startTimer += GameTime().GetFrameDeltaTime();
	//	return;
	//}

	//////////
	// ここから下が、ステージ開始時の演出の処理
	//////////

	if (CheckAButton == true && m_checkStartDirecting == true) {
		FallPlayer();
	}


	//if (g_pad[0]->IsTrigger(enButtonA) == true) {
		//CheckAButton = true;
		//checkZoomIn = true;
		//countZoomIn = startDirectingTime;

		//m_gameCamera->FinishZoom();
		//カメラの寄る処理のフラグをtrueにする。
		//m_gameCamera->SetStartDirectingZoomInCamera(true);
		//FallPlayer();
		//m_checkStartDirecting = false;
		////プレイヤーの操作フラグをtrueにする。
		//Player* player = FindGO<Player>("Player");
		//player->SetOperationFlag(true);
		////ポーズの操作フラグをtrueにする。
		//CPause* pause = FindGO<CPause>("Pause");
		//pause->SetCanPause(true);
		////UFOがあったら動かす
		//CUFO* ufo = FindGO<CUFO>("UFO");
		//if (ufo)
		//	ufo->SetMoveSpeed();
	//}

	//if (checkZoomIn == false) {
		//ここで大体のスタート演出をしている。
	


		//ここから下は使わない。

		//カメラの移動力
		//m_padLStickXF = 1.0f;
		//m_padLStickXF = 1.3f /** GameTime().GetFrameDeltaTime()*/;
		//if (m_padLStickXF > 1.5f)
		//	m_padLStickXF = 1.5f;

		////ウェイポイントの更新処理
		//CheckWayPoint();
		////移動処理
		//Move();
		////モデルの回転処理
		//Rotation();

		////デルタタイムを掛ける
		//m_onWayPosition += m_moveSpeed * GameTime().GetFrameDeltaTime();
		//GetOnStage();
	//}
	//else {
	//	if (countZoomIn < startDirectingTime) {
	//		++countZoomIn;
	//	}
	//	else {
	//		m_gameCamera->FinishZoom();
	//		//FallPlayer();
	//		//m_checkStartDirecting = false;
	//		////プレイヤーの操作フラグをtrueにする。
	//		//m_player = FindGO<Player>("Player");
	//		//player->SetOperationFlag(true);
	//		////ポーズの操作フラグをtrueにする。
	//		//CPause* pause = FindGO<CPause>("Pause");
	//		//pause->SetCanPause(true);
	//		////UFOがあったら動かす
	//		//CUFO* ufo = FindGO<CUFO>("UFO");
	//		//if (ufo)
	//		//	ufo->SetMoveSpeed();
	//	}
	//}
}


//void StartDirecting::CheckWayPoint()
//{
//	///
//	///
//	/// m_wayPointStateは左周りで0から増えていく。
//	/// m_wayPointStateが0の時、m_lpIndexは0,m_rpIndexは1になる。
//	/// つまり、m_lpIndexは常にm_wayPointStateと同じ値になり、
//	/// m_rpIndexはm_wayPointStateに1加算した値になる。
//	/// そしてm_maxWayPointStateはm_wayPointStateの最大数を表す。
//	/// m_maxWayPointStateが31だったらm_wayPointStateは31まで存在する。
//
//
//	//1.プレイヤー自身の左右のウェイポイントを設定する
//	//m_wayPointStateをもとにウェイポイントを設定する。
//	m_rpIndex = m_wayPointState;	//右のウェイポイントはとm_wayPointStateは同じ値
//	m_lpIndex = m_rpIndex + 1;		//左のウェイポイントは右のウェイポイントの1つ上の値
//
//	if (m_lpIndex > m_maxWayPointState)
//	{
//		//左のウェイポイントがMAXより大きかったら
//		//一周したということだから、スタートの0にする
//		m_lpIndex = 0;
//	}
//
//
//	//2.m_wayPointStateの更新。
//
//	Vector3 LpToRpVec = (*m_wayPointPos)[m_rpIndex] - (*m_wayPointPos)[m_lpIndex];
//	LpToRpVec.Normalize();
//	Vector3 LpToPlayerVec = m_position - (*m_wayPointPos)[m_lpIndex];
//	LpToPlayerVec.Normalize();
//	Vector3 RpToPlayerVec = m_position - (*m_wayPointPos)[m_rpIndex];
//	RpToPlayerVec.Normalize();
//
//	float LpDotPlayer = Dot(LpToRpVec, LpToPlayerVec);
//	float RpDotPlayer = Dot(LpToRpVec, RpToPlayerVec);
//
//
//	//左右のウェイポイントとの距離を調べる
//	float f = 0.35f;
//	if (LpDotPlayer > f && RpDotPlayer < -f)
//	{
//		//今のウェイポイントの間にいる
//	}
//	else if (LpDotPlayer <= f && m_padLStickXF < 0.0f)
//	{
//		//今のウェイポイントの間から、左側に出ていった
//		m_wayPointState += 1;
//		if (m_wayPointState > m_maxWayPointState)
//		{
//			//m_wayPointStateがMAXより大きかったら
//			//一周したということだから、スタートの0にする
//			m_wayPointState = 0;
//		}
//	}
//	else if (RpDotPlayer >= -f && m_padLStickXF > 0.0f)
//	{
//		//今のウェイポイントの間から、右側から出ていった
//		//m_wayPointStateを減算して右に進める。
//		m_wayPointState -= 1;
//		if (m_wayPointState < 0)
//		{
//			//m_wayPointStateが0より小さかったら
//			//一周したということだから、MAXの値にする
//			m_wayPointState = m_maxWayPointState;
//		}
//
//		//カメラの移動を止める位置に来たかの確認
//		if (m_wayPointState == 15) {
//			checkZoomIn = true;
//			//カメラの寄る処理のフラグをtrueにする。
//			m_gameCamera->SetStartDirectingZoomInCamera(true);
//		}
//	}
//
//
//
//	return;
//
//}
//
//void StartDirecting::Move()
//{
//	//移動する向きは毎フレーム計算した方がいいのかな？
//	//それとも、m_wayPointStateの切り替の時にした方がいいのかな？
//	//いや、今のやり方だと毎フレームやらなくてはいけない気がする
//
//	//1.左右への移動する方向を計算する。
//
//	//左へ移動する方向
//	Vector3 moveToLeft = (*m_wayPointPos)[m_lpIndex] - m_onWayPosition;
//	moveToLeft.Normalize();
//	//右へ移動する方向
//	Vector3 moveToRight = (*m_wayPointPos)[m_rpIndex] - m_onWayPosition;
//	moveToRight.Normalize();
//
//
//	//2.移動処理
//
//	//とりあえずの処理
//	//重力や、加速度、抵抗を実装するときは別のやり方で
//	m_moveSpeed = g_vec3Zero;
//
//	//カメラの移動スピード
//	float moveLen = 1000.0f;
//
//
//	if (m_padLStickXF < 0.0f)
//	{
//		//左への移動の入力があったら
//		//左への移動の計算する
//		m_moveSpeed += moveToLeft * m_padLStickXF * -moveLen;
//	}
//	else if (m_padLStickXF > 0.0f)
//	{
//		//右への移動の入力があったら
//		//右への移動の計算をする
//		m_moveSpeed += moveToRight * m_padLStickXF * moveLen;
//	}
//
//
//	return;
//}
//
//void StartDirecting::GetOnStage()
//{
//	m_upVec = g_vec3AxisY;
//	m_finalWPRot.Apply(m_upVec);
//	m_upVec.Scale(150.0f);
//	if (m_mobius)
//	{
//		if (m_mobius->GetModel()->InIntersectLine(m_onWayPosition + m_upVec, m_onWayPosition - m_upVec))
//		{
//			m_position = m_mobius->GetModel()->GetIntersectPos();
//		}
//	}
//	else
//	{
//		m_mobius = FindGO<Mobius>("Mobius");
//	}
//	auto hitPos = m_mobius->GetModel()->GetIntersectPos();
//
//	//m_modelRender->SetPosition(hitPos);
//	return;
//
//}
//
//void StartDirecting::Rotation()
//{
//	//左のウェイポイントから右のウェイポイントへのベクトル
//	Vector3 lpToRpLen = (*m_wayPointPos)[m_rpIndex] - (*m_wayPointPos)[m_lpIndex];
//
//	//左のウェイポイントからプレイヤーへのベクトル
//	Vector3 lpToPlayerLen = m_onWayPosition - (*m_wayPointPos)[m_lpIndex];
//
//	//補完率
//	float ComplementRate = lpToPlayerLen.Length() / lpToRpLen.Length();
//
//	//球面線形補完
//	m_finalWPRot.Slerp(ComplementRate, (*m_wayPointRot)[m_lpIndex], (*m_wayPointRot)[m_rpIndex]);
//
//	return;
//}

void StartDirecting::FallPlayer()
{
	m_player->SetFallFlag(true);
	Vector3 fallpos = m_player->GetPosition();
	fallpos.y -= 1200.0f * GameTime().GetFrameDeltaTime();
	if (fallpos.y <= 1660.0f) {
		fallpos.y = 1660.0f;

		if (m_fallTimer > 1.0f) {
			m_player->SetFallFlag(false);
			m_checkStartDirecting = false;
			//プレイヤーの操作フラグをtrueにする。
			m_player->SetOperationFlag(true);
			//ポーズの操作フラグをtrueにする。
			CPause* pause = FindGO<CPause>("Pause");
			pause->SetCanPause(true);
			//UFOがあったら動かす
			CUFO* ufo = FindGO<CUFO>("UFO");
			if (ufo)
				ufo->SetMoveSpeed();
		}
		else {
			m_fallTimer += GameTime().GetFrameDeltaTime();
		}
	}
	m_player->SetPosition(fallpos);
	m_gameCamera->SetTarget(m_player->GetPosition());
	m_gameCamera->SetPosition({ m_player->GetPositionX(),
								m_player->GetPositionY(),
								m_player->GetPositionZ() + 1200.0f });
}


//void StartDirecting::SetWayPointPos
//(const std::size_t vecSize, std::vector<Vector3>* const posMap)
//{
//	//vectorのサイズの確保
//	//m_wayPointPos->resize(vecSize);
//	//ウェイポイントステートの最大の値を設定
//	m_maxWayPointState = vecSize - 1;
//	//m_wayPointPosにウェイポイントの「場所」を格納する
//	m_wayPointPos = posMap;
//	//std::vector<Vector3>::iterator it = posMap->begin();
//	//for (int index = 0; it != posMap->end(); index++, it++)
//	//{
//	//	m_wayPointPos[index] = &it;
//	//}
//}
//
//void StartDirecting::SetWayPointRot
//(const std::size_t vecSize, std::vector<Quaternion>* rotMap)
//{
//	//vectorのサイズの確保
//	//m_wayPointRot->resize(vecSize);
//	//ウェイポイントステートの最大の値を設定
//	m_maxWayPointState = vecSize - 1;
//	//m_wayPointRotにウェイポイントの「回転」を格納する
//	m_wayPointRot = rotMap;
//	//std::map<int, Quaternion>::iterator it = rotMap->begin();
//	//for (int index = 0; it != rotMap->end(); index++, it++)
//	//{
//	//	m_wayPointRot[index] = &it->second;
//	//}
//}