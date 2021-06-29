#include "stdafx.h"
#include "OOwall.h"
#include "ROrunning_stop.h"

//�R���X�g���N�^
OOwall::OOwall()
{
	//�X�^�[�g�֐��ł͊Ԃɍ���Ȃ��B
	//�I�u�W�F�N�g���������ꂽ�u�ԂɌĂ΂�Ăق�������

	//�E�F�C�|�C���g����̏�̋�����ݒ�
	SetYPosLen(50.0f);
}

//�X�^�[�g�֐�
bool OOwall::StartSub()
{
	//�������p�֐�
	Init("Assets/modelData/wall.tkm", EN_OO_TYPE_WALL);

	//OBB�̃T�C�Y��ݒ�
	Vector3 obbSize;
	obbSize = { 400.0f,1000.0f,400.0f };
	SetOBBDirectionLength(obbSize);

	//�ړ��O�̏����ʒu�̐ݒ�
	m_startPosition = m_position;

	//�ғ�����Г����̎���
	const int moveTime = 3.0f;
	//�ړ����鋗���̕␳
	const float moveLen = 200.0f;
	//�A�b�v�x�N�g��
	Vector3 upVec = g_VEC3_UP;
	//���݂̎��g�̉�]�ŁA�A�b�v�x�N�g������
	m_rotation.Apply(upVec);
	//�A�b�v�x�N�g��
	upVec.Scale(moveLen * moveTime);
	//�ړ���̏I�[�ʒu�̐ݒ�
	m_endPosition = m_startPosition + upVec;

	SetOBBTag(COBB::EN_OO_TYPE_WALL);

	//�ŏ��ɓ���Ă������ғ��A��~�I�u�W�F�N�g��
	//��~�Ő�������
	m_pRun_stop = NewGO<ROrunning_stop>(0);
	m_pRun_stop->SetPosition(m_position);
	m_pRun_stop->SetFrontOrBack(EN_BACK);	
	//�S���]���Ȃ��悤�ɂƁATips��\�����Ȃ��悤�ɂ���
	m_pRun_stop->SetLock(true);

	//m_wallmoveSE�̃T�E���h�L���[�𐶐�����
	m_wallmoveSE = NewGO<CSoundCue>(0);
	//m_wallmoveSE�̃T�E���h�L���[���Awave�t�@�C�����w�肵�ď���������B
	m_wallmoveSE->Init(L"Assets/sound/wallmove.wav");
	//���ʒ���
	m_wallmoveSE->SetVolume(0.1f);

	SetYPosLen(100.0f);

	return true;
}

OOwall::~OOwall()
{
	DeleteGO(m_wallmoveSE);
}

//�A�b�v�f�[�g�֐�
void OOwall::UpdateSub()
{
	if (m_firstUpdateFlag)
		FirstUpdate();

	//�ғ������H
	if (m_moveFlag)
	{
		//�ғ�����Г����̎���
		const float moveTime = 3.0f;

		//�����ʒu����I�[�ʒl�ւ̃x�N�g��
		Vector3 movePos = m_endPosition - m_startPosition;	//�I�[�ʒu�ւ̃x�N�g��
		
		//���݂̎��� / �Г����̎��� �Ŕ{�����o��
		float moveScale = static_cast<float>(m_moveTimer) / moveTime;	//�ړ����x�̔{��

		//�J�E���^�[���Г����̎��Ԃ��z���Ă�����
		if (m_moveTimer >= moveTime)
		{
			//�I�[�ʒu�ւ̎c��̃J�E���^�[���o���āA�{�����o��
			moveScale = static_cast<float>(moveTime * 2 - m_moveTimer) / moveTime;

			//�J�E���^�[���������̎��Ԃ��z���Ă�����
			if (m_moveTimer >= moveTime * 2)
			{
				//�J�E���^�[��0�ɂ���
				m_moveTimer = 0;
			}
		}

		//�ړ���ւ̃x�N�g���ɁA�{�����|����
		movePos.Scale(moveScale);

		//���݂̏ꏊ���A�����ʒu����ړ���ւ̃x�N�g�������Z�����ꏊ�ɂ���
		m_position = m_startPosition + movePos;

		//�J�E���^�[��i�߂�
		m_moveTimer += GameTime().GetFrameDeltaTime();


		MoveSE();
	}

	return;
}

//�v���C���[��UFO�ɋ߂Â��Ɖ���炷
void OOwall::MoveSE()
{
	Vector3 distance = m_position - m_player->GetPosition();
	const float MaxDist = 1500;
	const float DistLen = distance.Length();

	if (DistLen < MaxDist) {
		float Le = MaxDist - DistLen;
		float SubLe = Le / MaxDist;
		float Vo = 2.0f * SubLe;

		//UFOmoveSE�����[�v�Đ����I���ōĐ�����B
		m_wallmoveSE->Play(true);

		//���ʒ���
		m_wallmoveSE->SetVolume(Vo);

	}
	else {
		if (m_wallmoveSE->IsPlaying()) {
			m_wallmoveSE->Stop();
		}
	}
}

/// <summary>
/// ���ڂ̃A�b�v�f�[�g�ł����Ă΂��֐�
/// </summary>
void OOwall::FirstUpdate()
{
	//�ғ��A��~�I�u�W�F�N�g���N�G������ƁA
	//m_pRun_stop�̒��g�̃I�u�W�F�N�g�������Ă��܂�����A
	//�������񃍁[�J���ϐ��̃|�C���^�ɔ����Ă�����
	//m_pRun_stop�ɂ�nullptr�����Ă���
	ROrunning_stop* pRun_stop = m_pRun_stop;
	m_pRun_stop = nullptr;
	//�N�G�������ĕǂ�T��
	pRun_stop->QuerySub();

	//���ڂ̃A�b�v�f�[�g�̏I��
	m_firstUpdateFlag = false;
}