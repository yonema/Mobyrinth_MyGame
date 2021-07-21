#include "stdafx.h"
#include "OOgoal.h"

//ObstacleObject�̃��f���̃t�@�C���p�X��OBB�̃T�C�Y�̒萔�f�[�^���g�p�\�ɂ���
using namespace OOsFilepathAndObbSizeConstData;
//�S�[���̒萔�f�[�^���g�p�\�ɂ���
using namespace goalConstData;

//�X�^�[�g�֐�
bool OOgoal::StartSub()
{
	//�������p�֐�
	Init(MODEL_FILEPATH_GOAL, EN_OO_TYPE_GOAL);

	//OBB�̃T�C�Y��ݒ�
	SetOBBDirectionLength(SIZE_OBB_GOAL);

	//�G�t�F�N�g�̏���������
	InitEffect();

	//�X�v���C�g�̏���������
	InitSprite();

	//�T�E���h�̏���������
	InitSound();

	//��Q�I�u�W�F�N�g�́A�v���C���[���f�ʂ�
	//OBBWorld�Ɏ��g��OBB�̓o�^������������
	COBBWorld::GetInstance()->RemoveOBB(GetOBB());

	return true;
}

/**
 * @brief �G�t�F�N�g�̏���������
*/
void OOgoal::InitEffect()
{
	//�S�[�����̃G�t�F�N�g�𐶐�
	m_goalEF = NewGO<Effect>(PRIORITY_FIRST);
	//������
	m_goalEF->Init(EFFECT_FILEPAHT_GOAL);
	//���W�Ɖ�]�Ɗg�嗦��ݒ�
	m_goalEF->SetPosition(m_position);
	m_goalEF->SetRotation(m_rotation);
	m_goalEF->SetScale(EFFECT_SCALE_GOAL);

	return;
}

/**
 * @brief �X�v���C�g�̏���������
*/
void OOgoal::InitSprite()
{
	//�S�[���̃X�v���C�g�̐���
	m_goalSR = NewGO<CSpriteRender>(PRIORITY_SECOND);
	//������
	m_goalSR->Init(
		SPRITE_FILEPATH_GOAL,
		SPRITE_WIDHT_GOAL,
		SPRITE_HEIGHT_GOAL,
		spriteRenderConstData::SPRITE_PIVOT_DEFALUT,
		AlphaBlendMode_Trans
	);
	//���W�ݒ�
	m_goalSR->SetPosition(SPRITE_POSITION_GOAL);
	m_goalSR->SetPostRenderFlag(true);

	//����������
	m_goalSR->Deactivate();

	return;
}

/**
 * @brief �T�E���h�̏���������
*/
void OOgoal::InitSound()
{
	//�S�[�����̃T�E���h�𐶐�����
	m_goalSC = NewGO<CSoundCue>(PRIORITY_FIRST);
	//�S�[�����̃T�E���h���Awave�t�@�C�����w�肵�ď���������B
	m_goalSC->Init(SOUND_FILEPATH_SE_GOAL);
	//���ʒ���
	m_goalSC->SetVolume(SOUND_VOLUME_SE_GOAL);

	return;
}

//�f�X�g���N�^
OOgoal::~OOgoal()
{
	//���\�[�X�̔j��
	DeleteGO(m_goalEF);
	DeleteGO(m_goalSR);
	DeleteGO(m_goalSC);

	return;
}

//�A�b�v�f�[�g�֐�
void OOgoal::UpdateSub()
{
	//���������H
	if (m_player->GetFallFalg())
		//�������̓S�[����������Ȃ�
		return;

	//�S�[����Ԃł͂Ȃ����A����
	//���g�ƃv���C���[���Փ˂�����
	if (m_isGoal == false && IsHitPlayer())
	{
		//�S�[�����̏���
		Goal();
	}

	return;
}


/**
 * @brief �S�[�����̏���
*/
void OOgoal::Goal()
{
	//�S�[����Ԃɂ���
	m_isGoal = true;
	//�v���C���[���S�[����Ԃɂ���
	m_player->SetGoal(true);

	//���f���������Ȃ�����
	GetModelRender()->Deactivate();

	//�G�t�F�N�g�̍��W�Ɖ�]���v���C���[�̂��̂ɐݒ肷��
	m_goalEF->SetPosition(m_player->GetPosition());
	m_goalEF->SetRotation(m_player->GetFinalWPRot());
	//�S�[�����̃G�t�F�N�g���Đ�����
	m_goalEF->Play();

	//�S�[�����̃X�v���C�g��\������
	m_goalSR->Activate();

	//�S�[�����̃T�E���h�������V���b�g�Đ��ōĐ�����
	m_goalSC->Play(false);

	//�����I�u�W�F�N�g���H
	if (GetFlagTransparentObject())
	{
		//�^�C�}�[�̃t�H���g�������Ȃ�����
		SetTimerFontAitiveFlag(false);
	}

	return;
}
