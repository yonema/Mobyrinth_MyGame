#include "stdafx.h"
#include "OOflame.h"

//ObstacleObject�̃��f���̃t�@�C���p�X��OBB�̃T�C�Y�̒萔�f�[�^���g�p�\�ɂ���
using namespace OOsFilepathAndObbSizeConstData;
//���̒萔�f�[�^���g�p�\�ɂ���
using namespace flameConstData;

//�X�^�[�g�֐�
bool OOflame::StartSub()
{
	//���f���̏������ƃ^�C�v�̐ݒ�
	Init(MODEL_FILEPATH_FLAME, EN_OO_TYPE_FLAME);

	//OBB�̕����x�N�g���̒�����ݒ�
	SetOBBDirectionLength(SIZE_OBB_FLAME);

	//���Ȕ����F��ݒ�
	SetModelEmissionColor(g_VEC4_RED);

	//OBB�̃^�O��ݒ肷��
	SetOBBTag(COBB::EN_OO_TYPE_FLAME);

	
	//�|�C���g���C�g�̏���������
	InitPointLight();

	//�T�E���h�̏���������
	InitSound();

	//�G�t�F�N�g�̏���������
	InitEffect();

	return true;
}

/**
 * @brief �|�C���g���C�g�̏���������
*/
void OOflame::InitPointLight()
{
	//�|�C���g���C�g�𐶐�
	m_pointLight = NewGO<CPointLight>(PRIORITY_FIRST);
	Vector3 ptLigPos = m_position;
	Vector3 up = g_VEC3_UP;
	m_rotation.Apply(up);
	up.Scale(LENGTH_UP_POSITION_POINT_LIGHT);
	//�|�C���g���C�g�����g�̍��W����ɒu��
	ptLigPos += up;
	//�|�C���g���C�g�̍��W��ݒ�
	m_pointLight->SetPosition(ptLigPos);

	//�|�C���g���C�g�̃J���[��ݒ�
	m_pointLight->SetColor(COLOR_POINT_LIGHT);
	//�|�C���g���C�g�̉e���͈͂�ݒ�
	m_pointLight->SetRange(RANGE_POINT_LIGHT);

	return;
}

/**
 * @brief �T�E���h�̏���������
*/
void OOflame::InitSound()
{
	//���̏펞���Ă���T�E���h�𐶐�����
	m_flameSE = NewGO<CSoundCue>(PRIORITY_FIRST);
	//���̏펞���Ă���T�E���h���Awave�t�@�C�����w�肵�ď���������B
	m_flameSE->Init(SOUND_FILEPATH_FLAME);
	//���ʒ���
	m_flameSE->SetVolume(SOUND_VOLUME_FLAME);


	//���Ɛ��̔����T�E���h�𐶐�����
	m_flame_waterSE = NewGO<CSoundCue>(PRIORITY_FIRST);
	//���Ɛ��̔����T�E���h���Awave�t�@�C�����w�肵�ď���������B
	m_flame_waterSE->Init(SOUND_FILEPATH_FLAME_WATAER);
	//���ʒ���
	m_flame_waterSE->SetVolume(SOUND_VOLUME_FLAME_WATER);

	return;
}

/**
 * @brief �G�t�F�N�g�̏���������
*/
void OOflame::InitEffect()
{
	//���Ɛ��̔����G�t�F�N�g�쐬
	m_flame_waterEF = NewGO<Effect>(PRIORITY_FIRST);
	m_flame_waterEF->Init(EFFECT_FILEPATH_FLAME);
	m_flame_waterEF->SetScale(EFFECT_SCALE_FLAME);
	Vector3 upVec = g_VEC3_UP;
	m_rotation.Apply(upVec);
	//���̒l��ύX���č����𒲐߂���
	upVec.Scale(LENGTH_UP_POSITION_EFFECT);
	m_flame_waterEF->SetPosition(m_position + upVec);		//���W��n��
	m_flame_waterEF->SetRotation(m_rotation);

	return;
}

//�f�X�g���N�^
OOflame::~OOflame()
{
	DeleteGO(m_pointLight);

	DeleteGO(m_flameSE);

	DeleteGO(m_flame_waterSE);

	DeleteGO(m_flame_waterEF);

	return;
}

//�A�b�v�f�[�g�֐�
void OOflame::UpdateSub()
{
	//�v���C���[�����ɋ߂Â��ƔR���Ă鉹���o��
	Burn();

	//���̃I�u�W�F�N�g�̃q�b�g�t���O�������Ă��邩�H
	if (GetIsHitFlag())
	{
		//���C�g������
		m_pointLight->SetColor(COLOR_POINT_LIGHT);
	}
	//�I�u�W�F�N�g�������̎��Ƀq�b�g�t���O���܂�Ă���
	else
	{
		//���C�g������
		m_pointLight->SetColor(g_VEC4_BLACK);
	}

	return;
}

//�_���[�W���󂯂�
void OOflame::Damage()
{
	//HP�����炵�āA0�ȉ����H
	if (--m_hp <= 0)
	{
		//HP��0�ȉ���������A���̃I�u�W�F�N�g������
		DeleteGO(this);
	}
	else
	{
		//OBB�̃T�C�Y��HP�ɉ����ď���������
		Vector3 obbSize;
		obbSize = SIZE_OBB_FLAME;

		//m_hp��m_maxHp��int�^�B
		//int�^���m�̊���Z�����珬�����؂�̂Ă���
		//�]�������l��0�ɂȂ��Ă��܂��B
		//������Am_hp��float�ɃL���X�g����
		obbSize.Scale(static_cast<float>(m_hp + SIZE_COMPLEMENT_FLAME) / (HP_MAX + SIZE_COMPLEMENT_FLAME));
		SetOBBDirectionLength(obbSize);


		//���f���̃T�C�Y��HP�ɉ����ď���������
			//���ꂪ��݂̂����ɐ؂�̂ĂɂȂ�Ȃ����R��
			//m_hp / m_maxHp�����g_vec3One * m_hp���Ă΂�
			//Vector3�̉��Z�q�̃I�[�o�[���C�h�̈�����float�^������
			//m_hp���n�����Ƃ��ɋ����^�ϊ����N����float�^�ɂȂ邩��
			//���ꂪSetScale(g_VEC3_ONE * (m_hp / m_maxHp))
			//�������炤�܂������Ȃ��B
		SetScale(g_VEC3_ONE * (m_hp + SIZE_COMPLEMENT_FLAME) / (HP_MAX + SIZE_COMPLEMENT_FLAME));
	}

	//�G�t�F�N�g�Đ�
	m_flame_waterEF->Play();
	//�T�E���h�Đ�
	m_flame_waterSE->Play(false);

	return;
}	

//�v���C���[�����ɋ߂Â��ƔR���Ă鉹���o��
void OOflame::Burn()
{
	//�v���C���[���玩�g�ւ̃x�N�g��
	Vector3 fromPlayerVec = m_position - m_player->GetPosition();
	//�v���C���[����̋���
	const float distFromPlayer = fromPlayerVec.Length();

	//�������ő勗����菬�������H
	if (distFromPlayer < DISTANCE_MAX_FLAME_SOUND)
	{
		//���ʂ̔{��
		//distFromPlayer���������Ȃ�قǁA�{�����`1.0f�ɋ߂Â�
		const float volumeRate = (DISTANCE_MAX_FLAME_SOUND - distFromPlayer) / DISTANCE_MAX_FLAME_SOUND;
		//���ʌv�Z
		const float volume = SOUND_VOLUME_FLAME * volumeRate;
		
		//flameSE�����[�v�Đ����I���ōĐ�����B
		m_flameSE->Play(true);
		
		//���ʒ���
		m_flameSE->SetVolume(volume);
		
	}
	else 
	{
		//�������ő勗�����傫��

		//�Đ������H
		if (m_flameSE->IsPlaying())
		{
			//�Đ����Ȃ��~����
			m_flameSE->Stop();
		}
	}

	return;
}