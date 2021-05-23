#include "stdafx.h"
#include "GameCamera.h"
class Player;

bool GameCamera::Start()
{
	//�v���C���[��T��
	m_pPlayer = FindGO<Player>("Player");
	//�����_���王�_�ւ̃x�N�g����ݒ肷��
	m_toCameraPos = { 0.0f,0.0f,1000.0f };

	//�J�n���o�p�̃x�N�g��
	m_toCameraPos = { 0.0f,0.0f,4000.0f };

	//�΂˃J�����̏�����
	m_springCamera.Init(*g_camera3D, 5000.0f, false, 0.0f);

	if (m_pPlayer)
	{
		//�v���C���[������������
		//�v���C���[�̈ʒu�����ƂɃJ�����̏ꏊ�����߂�
		m_springCamera.SetPosition(m_pPlayer->GetPosition() + m_toCameraPos);
		m_springCamera.SetTarget(m_pPlayer->GetPosition());
	}
	else
	{
		//������Ȃ�������
		//�f�t�H���g�̈ʒu�ɂ���
		m_springCamera.SetPosition({ 0.0f,1800.0f,1000.0f });
		m_springCamera.SetTarget({ 0.0f,1800.0f,0.0f });
	}


	//�ߕ��ʁA�����ʂ̐ݒ�
	m_springCamera.SetNear(0.5f);
	m_springCamera.SetFar(50000.0f);

	//�X�e�[�W�J�n���̉��o��T��
	m_startDirecting = FindGO<StartDirecting>("StartDirecting");

	//�J�����̏�����̏C��
	//const Quaternion qRot = (m_pPlayer->GetFinalWPRot());
	//qRot.Apply(m_toCameraPos);
	Vector3 vecUp = g_vec3AxisY;
	//qRot.Apply(vecUp);

	m_springCamera.SetUp(vecUp);


	//�t�F�[�h�Ɏg���X�v���C�g�̐����Ə�����
	m_fadeSR = NewGO<CSpriteRender>(0);
	m_fadeSR->Init("Assets/Image/black.DDS", 1280.0f, 780.0f, { 0.5f,0.5f }, AlphaBlendMode_Trans);
	m_fadeSR->SetPosition({ 0.0f,0.0f,0.0f });
	m_fadeSR->SetPostRenderFlag(true);
	//�����ɂ��Ă���
	m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
	//��\���ɂ���
	//m_fadeSR->Deactivate();


	//�J�n���o�p�̈ʒu���
	m_springCamera.SetPosition(m_toCameraPos);
	m_springCamera.SetTarget({ 0.0f,0.0f,0.0f });

	return true;
}

void GameCamera::Update()
{
	if (m_startDirecting->GetStartDirecting() == true) {
		if (m_startDirecting->GetCheckAButton() == false) {
			StartDirectingCamera();
		}
		if (m_startDirectingZoomInCamera == true) {
			FadeDirectingCamera();
		}
	}
	else {
		InGameCamera();
	}

	//�΂˃J�����̍X�V
	m_springCamera.Update();
}

void GameCamera::StartDirectingCamera()
{
	if (m_startDirecting->GetStartDirecting() == false || !m_wipeEndFlag) {
		return;
	}

	//������Ƒ҂��Ă��牉�o���J�n����
	if (m_startTimer < /*0.5f*/0.5f)
	{
		m_startTimer += GameTime().GetFrameDeltaTime();
		return;
	}



	if (m_flagRotationCamera == true) {
		const Quaternion qRot = { 0.0f,0.005f,0.0f,1.0f };
		//�J�����ւ̃x�N�g������
		qRot.Apply(m_toCameraPos);
		//�X�e�[�W�S�̂����n����ݒ�
		m_springCamera.SetPosition(m_toCameraPos);
		m_springCamera.SetTarget({ 0.0f,0.0f,0.0f });
	}


	if (m_startDirecting) {
		//�X�e�[�W�̎������]����B
		if (m_startDirectingZoomInCamera == false) {
			//�����I�Ƀ}�W�b�N�i���o�[�ɂȂ邯�ǁA���Ԃ��Ȃ��̂ł��̂܂܂ł����܂��B
			m_test = m_test + 0.0017f;

			if (m_test >= 1.0f || g_pad[0]->IsTrigger(enButtonA) == true) {
				m_startDirectingZoomInCamera = true;
			}

		}
		//�L�����N�^�[�̈ʒu�ɃJ���������킹��B
		else {
			

			//�������牺�́A�����g��Ȃ��B

			//if (m_zoomFinished == false) {
			//	Vector3 differenceCameraPos = { 0.0f,0.0f,1000.0f / m_startDirecting->GetStartDirectingTime() };
			//	//Vector3 cameraPos1 = { 0.0f,0.0f,2000.0f };
			//	//Vector3 cameraPos2= { 0.0f,0.0f,1000.0f };

			//	m_toCameraPos = m_toCameraPos - differenceCameraPos;

			//	g_camera3D->SetTarget(m_startDirecting->GetPosition());

			//	g_camera3D->SetPosition(m_startDirecting->GetPosition() + m_toCameraPos);
			//}
			//else
			//{
			//	g_camera3D->SetTarget({ 0.0f,1800.0f,0.0f });
			//	g_camera3D->SetPosition({ 0.0f,1800.0f,1200.0f });
			//}
		}
	}
	else {
		//�X�e�[�W�J�n���̉��o���������Ă��Ȃ�������
		//�X�e�[�W�J�n���̉��o��T��
		m_startDirecting = FindGO<StartDirecting>("StartDirecting");
	}

}

void GameCamera::FadeDirectingCamera()
{
	const float startWaitTime = 0.5f;							//�ŏ��̑҂���		 
	const float startFadeOutTime = startWaitTime + 0.5f;		//�ŏ��̃t�F�[�h�A�E�g����
	const float startFadeWaitTime = startFadeOutTime + 0.5f;	//�ŏ��̃t�F�[�h���Ԏ���
	const float startFadeInTime = startFadeWaitTime + 0.5f;		//�ŏ��̃t�F�[�h�C������

	//�؂�ւ����̃^�C�}�[�̌o�߂𒲂ׂ�
	if (m_switchingTimer < startWaitTime)
	{
		//�ŏ��̑҂���
		int test = 10;
		//���������ɑ҂�
	}
	else if (m_switchingTimer < startFadeOutTime)
	{
		//�t�F�[�h�A�E�g���Ă���

		//�A���t�@�l
		float alphaValue = 1.0f;
		//�^�C�}�[�Ɍo�ߋ�ɂ���ĕ⊮
		float timeScale = (m_switchingTimer - startWaitTime) / (startFadeOutTime - startWaitTime);
		alphaValue *= timeScale;
		//�t�F�[�h�����X�ɈÂ����Ă���
		m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,alphaValue });
	}
	else if (m_switchingTimer < startFadeWaitTime)
	{
		//�t�F�[�h�A�E�g�ƃt�F�[�h�C���̊�
		//���������ɑ҂�

		//�J�������X�e�[�W�̎������]���Ȃ��悤�ɂ���B
		m_flagRotationCamera = false;

		//�t�F�[�h�͐^����
		m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });
	}
	else if (m_switchingTimer < startFadeInTime)
	{
		//�t�F�[�h�C�����Ă���

		//A�{�^���������������ɂ���B
		m_startDirecting->SetCheckAButton(true);

		//�J�������v���C���[������悤�ɂ���B
		/*SetTarget(m_pPlayer->GetPosition());
		SetPosition({ m_pPlayer->GetPositionX(),
					m_pPlayer->GetPositionY(),
					m_pPlayer->GetPositionZ() + 1200.0f });*/

		//�A���t�@�l
		float alphaValue = 1.0f;
		//�^�C�}�[�Ɍo�ߋ�ɂ���ĕ⊮
		float timeScale = (m_switchingTimer - startFadeWaitTime) / (startFadeInTime - startFadeWaitTime);
		alphaValue -= 1.0f * timeScale;
		//�t�F�[�h�����X�ɖ��邭���Ă���
		m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,alphaValue });
	}

	//�J���������t���b�V�����āA�ꎞ�I�ɂ΂˃J�����𖳌��ɂ��A
	//��C�ɃJ�������ړ��ł���悤�ɂ���
	m_springCamera.Refresh();

	m_switchingTimer += GameTime().GetFrameDeltaTime();
}

void GameCamera::InGameCamera()
{
	if (m_lookPlayerFlag)
	{
		if (m_pPlayer)
		{
			//�v���C���[���������Ă�����

			//�����_���王�_�ւ̃x�N�g����ݒ肷��
			m_toCameraPos = { 0.0f,0.0f,1200.0f };
			//if (m_titleMode)
			//	m_toCameraPos = { 0.0f,600.0f,1200.0f };
			const float upLen = 400.0f;
			Vector3 upVec = { 0.0f,upLen,0.0f };



			//�v���C���[�̉�]�𓾂�
			const Quaternion qRot = (m_pPlayer->GetFinalWPRot());
			//�J�����ւ̃x�N�g������
			qRot.Apply(m_toCameraPos);
			//�A�b�v�x�N�g��
			Vector3 vecUp = g_vec3AxisY;
			//�A�b�v�x�N�g������
			qRot.Apply(vecUp);

			qRot.Apply(upVec);

			if (!m_titleMode)
				upVec = g_vec3Zero;

			//�����_��ݒ肷��
			m_springCamera.SetTarget(m_pPlayer->GetPosition() + upVec);
			//���_��ݒ肷��
			m_springCamera.SetPosition(m_pPlayer->GetPosition() + m_toCameraPos + upVec);
			//�A�b�v����ݒ肷��
			m_springCamera.SetUp(vecUp);
		}
		else
		{
			//�v���C���[���������Ă��Ȃ�������
			//�v���C���[��T��
			m_pPlayer = FindGO<Player>("Player");
		}
	}


	return;
}