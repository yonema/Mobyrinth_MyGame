#include "stdafx.h"
#include "ROkey_padlock.h"

//�X�^�[�g�֐�
bool ROkey_padlock::StartSub()
{
	//�������p�֐�
	Init("Assets/modelData/key.tkm", EN_RO_TYPE_KEY,
		"Assets/modelData/padlock.tkm", EN_RO_TYPE_PADLOCK);



	//padlockbreakSE�̃T�E���h�L���[�𐶐�����
	m_padlockbreakSE = NewGO<CSoundCue>(0);
	//padlockbreakSE�̃T�E���h�L���[���Awave�t�@�C�����w�肵�ď���������B
	m_padlockbreakSE->Init(L"Assets/sound/padlockbreak.wav");
	//���ʒ���
	m_padlockbreakSE->SetVolume(1.0f);

	return true;
}

/// <summary>
/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐�
/// </summary>
void ROkey_padlock::QuerySub()
{
	//���g���u���̋��^�v�̎�
	if (GetObjectType() == EN_RO_TYPE_KEY)
	{
		//��Q�I�u�W�F�N�g�́u���ĂȂ��싞���v���N�G��
		QueryLOs<OOpadlock>(EN_OO_TYPE_BIG_PADLOCK, [&](OOpadlock* padlock) -> bool
			{
				//���g�Ɓu���ĂȂ��싞���v���Փ˂�����
				if (IsHitObject(*this, *padlock))
				{
					//�u���ĂȂ��싞���v��j��
					DeleteGO(padlock);

					//padlockbreakSE�����[�v�Đ����I�t�ōĐ�����B
					m_padlockbreakSE->Play(false);

					//���g�̃I�u�W�F�N�g��j��
					DeleteGO(this);
				}
				return true;
			}
		);
	}
	//else if (GetObjectType() == enROPadlock)
	//{
	//	QueryLOs<OObigFire>(EN_OO_TYPE_FLAME, [&](OObigFire* bigFire) -> bool
	//		{
	//			if (IsHitObject(*this, *bigFire, hitDot))
	//			{
	//				bigFire->Damage();
	//				DeleteGO(this);
	//			}
	//			return true;
	//		}
	//	);
	//}
}