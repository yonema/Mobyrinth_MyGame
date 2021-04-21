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

	//�X�e�[�W�J�n���̉��o��T��
	m_startDirecting = FindGO<StartDirecting>("StartDirecting");

	//�J�����̏�����̏C��
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
			//�����_���王�_�ւ̃x�N�g����ݒ肷��
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
		//�X�e�[�W�J�n���̉��o���������Ă��Ȃ�������
		//�X�e�[�W�J�n���̉��o��T��
		m_startDirecting = FindGO<StartDirecting>("StartDirecting");
	}
}

void GameCamera::InGameCamera()
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