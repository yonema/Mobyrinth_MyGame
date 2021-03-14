#include "stdafx.h"
#include "GameCamera.h"
class Player;

bool GameCamera::Start()
{
	//�v���C���[��T��
	m_pPlayer = FindGO<Player>("Player");
	//�����_���王�_�ւ̃x�N�g����ݒ肷��
	m_toCameraPos = { 0.0f,0.0f,1000.0f };


	if (m_pPlayer)
	{
		//�v���C���[������������
		//�v���C���[�̈ʒu�����ƂɃJ�����̏ꏊ�����߂�
		g_camera3D->SetPosition(m_pPlayer->GetPosition() + m_toCameraPos);
		g_camera3D->SetTarget(m_pPlayer->GetPosition());
	}
	else
	{
		//������Ȃ�������
		//�f�t�H���g�̈ʒu�ɂ���
		g_camera3D->SetPosition({ 0.0f,1800.0f,1000.0f });
		g_camera3D->SetTarget({ 0.0f,1800.0f,0.0f });
	}


	//�ߕ��ʁA�����ʂ̐ݒ�
	g_camera3D->SetNear(0.5f);
	g_camera3D->SetFar(50000.0f);

	return true;
}

void GameCamera::Update()
{
	if (m_pPlayer)
	{
		//�v���C���[���������Ă�����

		//�����_���王�_�ւ̃x�N�g����ݒ肷��
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
		//�v���C���[���������Ă��Ȃ�������
		//�v���C���[��T��
		m_pPlayer = FindGO<Player>("Player");
	}


	return;
}