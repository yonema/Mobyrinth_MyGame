#include "stdafx.h"
#include "GameCamera.h"
class Player_kari;

bool GameCamera::Start()
{
	m_pPlayer = FindGO<Player_kari>("player");
	m_toCameraPos = { 0.0f,0.0f,1000.0f };

	if (m_pPlayer)
	{
		g_camera3D->SetPosition(m_pPlayer->GetPosition() + m_toCameraPos);
		g_camera3D->SetTarget(m_pPlayer->GetPosition());
	}
	else
	{
		g_camera3D->SetPosition({ 0.0f,1800.0f,1000.0f });
		g_camera3D->SetTarget({ 0.0f,1800.0f,0.0f });
	}

	g_camera3D->SetNear(0.5f);
	g_camera3D->SetFar(50000.0f);

	return true;
}

void GameCamera::Update()
{
	if (m_pPlayer)
	{
		m_toCameraPos = { 0.0f,0.0f,1000.0f };

		g_camera3D->SetTarget(m_pPlayer->GetPosition());

		Quaternion qRot = m_pPlayer->GetFinalWPRot();
		qRot.Apply(m_toCameraPos);
		g_camera3D->SetPosition(m_pPlayer->GetPosition() + m_toCameraPos);
		Vector3 vecUp = g_vec3AxisY;
		qRot.Apply(vecUp);
		g_camera3D->SetUp(vecUp);
	}
	else
	{
		m_pPlayer = FindGO<Player_kari>("player");
	}


	return;
}