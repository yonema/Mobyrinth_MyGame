#include "stdafx.h"
#include "ROkey_padlock.h"

//ReversibleObject�̃��f���̃t�@�C���p�X�̒萔�f�[�^���g�p�\�ɂ���
using namespace ROsFilepathConstdata;
//�u���v�u�싞���v�̒萔�f�[�^���g�p�\�ɂ���
using namespace key_padlockConstData;

//�X�^�[�g�֐�
bool ROkey_padlock::StartSub()
{
	//�������p�֐�
	Init(MODEL_FILEPATH_KEY, EN_RO_TYPE_KEY,
		MODEL_FILEPATH_PADLOCK, EN_RO_TYPE_PADLOCK);

	//�T�E���h�̏���������
	InitSound();

	return true;
}

/**
 * @brief �T�E���h�̏���������
*/
void ROkey_padlock::InitSound()
{
	//�싞�����J���Ƃ��̃T�E���h�𐶐�����
	m_padlockbreakSE = NewGO<CSoundCue>(PRIORITY_FIRST);
	//�싞�����J���Ƃ��̃T�E���h���Awave�t�@�C�����w�肵�ď���������B
	m_padlockbreakSE->Init(SOUND_FILEPATH_BREAK_PADLOCK);
	//���ʒ���
	m_padlockbreakSE->SetVolume(SOUND_VOLUME_BREAK_PADLOCK);

	return;
}

/// <summary>
/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐�
/// </summary>
void ROkey_padlock::QuerySub()
{
	//���g���u���̋��^�v�̎�
	if (GetObjectType() == EN_RO_TYPE_KEY)
	{
		//��Q�I�u�W�F�N�g�́u�傫�ȓ싞���v���N�G��
		QueryLOs<OOpadlock>(EN_OO_TYPE_BIG_PADLOCK, [&](OOpadlock* bigPadlock) -> bool
			{
				//���g���u���v�ŁA�u�傫�ȓ싞���v�ƏՓ˂������̏���
				KeyHitBigPadlock(bigPadlock);

				return true;
			}
		);
	}

	return;

}


/**
 * @brief ���g���u���v�ŁA�u�傫�ȓ싞���v�ƏՓ˂������̏���
 * @param [in] bigPadlock �u�傫�ȓ싞���v�̃|�C���^
*/
void ROkey_padlock::KeyHitBigPadlock(OOpadlock* bigPadlock)
{
	//�s���ł��邩�H
	if (m_actionFlag)
	{
		//���g�Ɓu�傫�ȓ싞���v���Փ˂�����
		if (IsHitLevelObject(*this, *bigPadlock))
		{
			//�u�傫�ȓ싞���v��j��
			DeleteGO(bigPadlock);
			//�싞�����J���Ƃ��̃T�E���h�������V���b�g�Đ��ōĐ�����
			m_padlockbreakSE->Play(false);
			//�s���ł��Ȃ�����
			m_actionFlag = false;
			//���g�̃I�u�W�F�N�g��j��
			DeleteGO(this);
		}
	}

	return;
}