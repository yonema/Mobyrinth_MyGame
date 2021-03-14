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

	return true;
}

void GameCamera::Update()
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