#include "stdafx.h"
#include "StartDirecting.h"
#include "GameCamera.h"

//�J�n���o�̒萔�f�[�^���g����悤�ɂ���
using namespace startDirectingConstData;

/**
 * @brief �J�n���o�̍X�V�B�g���Ƃ��ɃA�b�v�f�[�g�֐��ŌĂԁB
*/
void CStartDirecting::StartDirectingUpdate()
{
	//�X�e�[�W�̎�������邮���邩�H
	if (m_goAroundStageFlag)
	{
		GoAroundStage();
	}

	//�t�F�[�h���s�����H
	if (m_fadeFlag)
	{
		FadeDirectingCamera();
	}

	//�v���C���[�������Ă��鉉�o���s�����H
	if (m_fallPlayerFlag)
	{
		FallPlayer();
	}

	return;
}


/**
 * @brief �X�e�[�W�J�n���̃J����
*/
void CStartDirecting::GoAroundStage()
{
	//���C�v���I����Ă��Ȃ����H
	if (!m_wipeEndFlag)
	{
		//�I����Ă��Ȃ�
		//��������return
		return;
	}

	//������Ƒ҂��Ă��牉�o���J�n����
	if (m_firstWaitTimer < TIME_FIRST_WAIT_START_DIRECTING)
	{
		//�^�C�}�[��i�߂�
		m_firstWaitTimer += GameTime().GetFrameDeltaTime();
		return;
	}

	//�X�e�[�W�̎�������N�H�[�^�j�I��
	Quaternion goAroundStageQRot;
	//�X�e�[�W�̎�������p�x�𑝂₷	//�f���^�^�C�����|����
	m_goAroundStageAngle += ANGLE_MAX_GO_AROUND_STAGE / TIME_GO_AROUND_STAGE * 
		CGameTime::GetInstance().GetFrameDeltaTime();
	//�N�H�[�^�j�I���Ɋp�x���Z�b�g����
	goAroundStageQRot.SetRotationDegY(m_goAroundStageAngle);
	//�J�����̎��_�ւ̍��W
	Vector3 toCameraPos = POSITION_TO_CAMERA;
	//�J�����̎��_�ւ̍��W����
	goAroundStageQRot.Apply(toCameraPos);
	//�X�e�[�W�S�̂����n����悤�Ɏ��_�ƒ����_��ݒ�
	m_gameCamera->SetPosition(POSITION_TARGET_CAMERA + toCameraPos);
	m_gameCamera->SetTarget(POSITION_TARGET_CAMERA);

	//�X�e�[�W�̎�������p�x�����̊p�x�ɒB����A�܂���
	//A�{�^��������
	if (m_goAroundStageAngle >= ANGLE_MAX_GO_AROUND_STAGE * SCALE_ANGLE_FINISH_GO_AROUND_STAGE ||
		g_pad[0]->IsTrigger(enButtonA) == true)
	{
		//�t�F�[�h�̃t���O�𗧂Ă�
		m_fadeFlag = true;
	}

	return;
}

/**
 * @brief �t�F�[�h�̏���
*/
void CStartDirecting::FadeDirectingCamera()
{
	//�t�F�[�h�A�E�g�̎��Ԃ��H
	if (m_fadeTimer < TIME_FADE_OUT)
	{
		//�A���t�@�l
		float alphaValue = 1.0f;
		//�^�C�}�[�Ɍo�ߋ�ɂ���ăA���t�@�l��⊮
		float timeScale = (m_fadeTimer) / (TIME_FADE_OUT);
		alphaValue *= timeScale;
		//�t�F�[�h�����X�ɈÂ����Ă���
		g_graphicsEngine->GetHUD().SetFadeSpriteAlphaValue(alphaValue);
	}
	//�t�F�[�h�A�E�g�ƃt�F�[�h�C���̒��Ԃ̎��Ԃ��H
	else if (m_fadeTimer < TIME_FADE_WAIT)
	{
		//�J�������X�e�[�W�̎������]���Ȃ��悤�ɂ���B
		m_goAroundStageFlag = false;

		//�t�F�[�h�͐^����
		g_graphicsEngine->GetHUD().
			SetFadeSpriteAlphaValue(spriteRenderConstData::ALPHA_VALUE_OPACITY);
	}
	//�t�F�[�h�C���̎��Ԃ��H
	else if (m_fadeTimer < TIME_FADE_IN)
	{
		//�v���C���[�������Ă��鉉�o���n�߂�
		m_fallPlayerFlag = true;

		//�A���t�@�l
		float alphaValue = 1.0f;
		//�^�C�}�[�Ɍo�ߋ�ɂ���ĕ⊮
		float timeScale = (m_fadeTimer - TIME_FADE_WAIT) / (TIME_FADE_IN - TIME_FADE_WAIT);
		alphaValue -= 1.0f * timeScale;
		//�t�F�[�h�����X�ɖ��邭���Ă���
		g_graphicsEngine->GetHUD().SetFadeSpriteAlphaValue(alphaValue);
	}
	else
	{
		//�S�Ă̍H�����I�������
		//�t�F�[�h�����S�ɓ����ɂ���
		g_graphicsEngine->GetHUD().
			SetFadeSpriteAlphaValue(spriteRenderConstData::ALPHA_VALUE_TRANSPARENT);
		//�t�F�[�h���s��Ȃ��悤�ɂ���
		m_fadeFlag = false;
	}

	//�t�F�[�h�̃^�C�}�[��i�߂�
	m_fadeTimer += GameTime().GetFrameDeltaTime();

	return;
}

/**
 * @brief �v���C���[�̉��o���̗�������
*/
void CStartDirecting::FallPlayer()
{
	//�v���C���[���݂����Ă��Ȃ����H
	if (m_player == nullptr)
	{
		//�������Ă��Ȃ�
		//�v���C���[��T��
		m_player = FindGO<Player>(GetGameObjectName(EN_GO_TYPE_PLAYER));
		//�܂��������Ă��Ȃ�������
		if (m_player == nullptr)
		{
			//����������return
			return;
		}
	}

	//�v���C���[�𗎂��Ă����Ԃɂ���
	m_player->SetFallFlag(true);

	//�������̃v���C���[�̍��W
	Vector3 fallpos = m_player->GetPosition();
	//����������	//�f���^�^�C�����|����
	fallpos.y -= SPEED_FALL_PLAYER * GameTime().GetFrameDeltaTime();
	//�������n�ʂ��Ⴂ���H
	if (fallpos.y < HEIGHT_GROUND) 
	{
		//������n�ʂɍ��킹��
		fallpos.y = HEIGHT_GROUND;

		//������ɑ҂��Ԃ��H
		if (m_waitTimerAfterFell < TIME_WAIT_AFTER_FELL) 
		{
			//������ɏ����ҋ@����
			m_waitTimerAfterFell += GameTime().GetFrameDeltaTime();
		}
		//�����҂�����
		else 
		{
			//�J�n���o���I����
			m_startDirectingFlag = false;

			//�v���C���[�𗎂��Ă����Ԃł͂Ȃ�����
			m_player->SetFallFlag(false);
			//�v���C���[�̑���t���O��true�ɂ���B
			m_player->SetOperationFlag(true);

			//�|�[�Y�̑���t���O��true�ɂ���B
			CPause* pause = FindGO<CPause>(GetGameObjectName(EN_GO_TYPE_PAUSE));
			pause->SetCanPause(true);
			//UFO���������瓮����
			CUFO* ufo = FindGO<CUFO>(GetGameObjectName(EN_GO_TYPE_UFO));
			if (ufo)
				ufo->SetMoveSpeed();
		}
	}

	//�v���C���[�ɗ������̍��W��ݒ肷��
	m_player->SetPosition(fallpos);
	//�J���������t���b�V�����āA�ꎞ�I�ɂ΂˃J�����𖳌��ɂ��A
	//��C�ɃJ�������ړ��ł���悤�ɂ���
	m_gameCamera->Refresh();
	//�J�����̎��_��ݒ肷��
	m_gameCamera->SetPosition(m_player->GetPosition() + gameCameraConstData::POSITION_TO_CAMERA);
	//�J�����̒����_��ݒ肷��
	m_gameCamera->SetTarget(m_player->GetPosition());

	return;
}
