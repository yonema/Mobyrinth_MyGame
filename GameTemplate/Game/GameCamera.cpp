#include "stdafx.h"
#include "GameCamera.h"
class Player;

bool GameCamera::Start()
{
	//プレイヤーを探す
	m_pPlayer = FindGO<Player>("Player");
	//注視点から視点へのベクトルを設定する
	m_toCameraPos = { 0.0f,0.0f,1000.0f };


	if (m_pPlayer)
	{
		//プレイヤーが見つかったら
		//プレイヤーの位置をもとにカメラの場所を決める
		g_camera3D->SetPosition(m_pPlayer->GetPosition() + m_toCameraPos);
		g_camera3D->SetTarget(m_pPlayer->GetPosition());
	}
	else
	{
		//見つからなかったら
		//デフォルトの位置にする
		g_camera3D->SetPosition({ 0.0f,1800.0f,1000.0f });
		g_camera3D->SetTarget({ 0.0f,1800.0f,0.0f });
	}


	//近平面、遠平面の設定
	g_camera3D->SetNear(0.5f);
	g_camera3D->SetFar(50000.0f);

	//ステージ開始時の演出を探す
	m_startDirecting = FindGO<StartDirecting>("StartDirecting");

	//カメラの上方向の修正
	//const Quaternion qRot = (m_pPlayer->GetFinalWPRot());
	//qRot.Apply(m_toCameraPos);
	Vector3 vecUp = g_vec3AxisY;
	//qRot.Apply(vecUp);

	g_camera3D->SetUp(vecUp);

	return true;
}

void GameCamera::Update()
{
	if (m_startDirecting->GetStartDirecting() == true) {
		StartDirectingCamera();
	}
	else {
		InGameCamera();
	}
}

void GameCamera::StartDirectingCamera()
{
	if (m_startDirecting) {
		if (m_startDirectingZoomInCamera == false) {
			//注視点から視点へのベクトルを設定する
			m_toCameraPos = { 0.0f,0.0f,2000.0f };

			//const Quaternion qRot = (m_startDirecting->GetFinalWPRot());
			//qRot.Apply(m_toCameraPos);
			//Vector3 vecUp = g_vec3AxisY;
			//qRot.Apply(vecUp);

			g_camera3D->SetTarget(m_startDirecting->GetPosition());

			g_camera3D->SetPosition(m_startDirecting->GetPosition() + m_toCameraPos);

			//g_camera3D->SetUp(vecUp);

		}
		else {
			Vector3 differenceCameraPos = { 0.0f,0.0f,1000.0f / m_startDirecting->GetStartDirectingTime() };
			//Vector3 cameraPos1 = { 0.0f,0.0f,2000.0f };
			//Vector3 cameraPos2= { 0.0f,0.0f,1000.0f };

			m_toCameraPos = m_toCameraPos - differenceCameraPos;

			g_camera3D->SetTarget(m_startDirecting->GetPosition());

			g_camera3D->SetPosition(m_startDirecting->GetPosition() + m_toCameraPos);
		}
	}
	else {
		//ステージ開始時の演出が見つかっていなかったら
		//ステージ開始時の演出を探す
		m_startDirecting = FindGO<StartDirecting>("StartDirecting");
	}
}

void GameCamera::InGameCamera()
{
	if (m_pPlayer)
	{
		//プレイヤーが見つかっていたら

		//注視点から視点へのベクトルを設定する
		m_toCameraPos = { 0.0f,0.0f,1000.0f };

		const Quaternion qRot = (m_pPlayer->GetFinalWPRot());
		qRot.Apply(m_toCameraPos);
		Vector3 vecUp = g_vec3AxisY;
		qRot.Apply(vecUp);


		g_camera3D->SetTarget(m_pPlayer->GetPosition());

		g_camera3D->SetPosition(m_pPlayer->GetPosition() + m_toCameraPos);

		g_camera3D->SetUp(vecUp);
	}
	else
	{
		//プレイヤーが見つかっていなかったら
		//プレイヤーを探す
		m_pPlayer = FindGO<Player>("Player");
	}


	return;
}