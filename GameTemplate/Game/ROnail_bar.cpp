#include "stdafx.h"
#include "ROnail_bar.h"

//�X�^�[�g�֐�
bool ROnail_bar::StartSub()
{
	//�������p�֐�
	Init("Assets/modelData/nail.tkm", EN_RO_TYPE_NAIL,
		"Assets/modelData/hammer.tkm", EN_RO_TYPE_BAR);

	//����Ƃ��̃T�E���h�̐����Ə�����
	m_brokenSE = NewGO<CSoundCue>(0);
	m_brokenSE->Init(L"Assets/sound/boxBreak.wav");
	m_brokenSE->SetVolume(0.3f);

	//����Ƃ��̃G�t�F�N�g�̐����Ə�����
	m_effect = NewGO<Effect>(0);
	m_effect->Init(u"Assets/effect/flame_water.efk");
	float scale = 200.0f;
	m_effect->SetScale({ scale,scale ,scale });


	return true;
}

//�f�X�g���N�^
ROnail_bar::~ROnail_bar()
{
	DeleteGO(m_brokenSE);
	DeleteGO(m_effect);
}

/// <summary>
/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐�
/// </summary>
void ROnail_bar::QuerySub()
{
	//���g���u�B�v�̎�
	if (GetObjectType() == EN_RO_TYPE_NAIL)
	{

	}
	//���g���u�o�[���v�̎�
	else if (GetObjectType() == EN_RO_TYPE_BAR)
	{
		////�u���ĂȂ��싞���v�Ɓu���v���ƁA�u���ĂȂ��싞���v�̕����D��I�ɔj�����邩��
		////�u���ĂȂ��싞���v���ɃN�G������
		////��Q�I�u�W�F�N�g�́u���ĂȂ��싞���v���N�G��
		//QueryLOs<OOpadlock>(EN_OO_TYPE_BIG_PADLOCK, [&](OOpadlock* padlock) -> bool
		//	{
		//		//���g�Ɓu���ĂȂ��싞���v���Փ˂�����
		//		if (IsHitObject(*this, *padlock))
		//		{
		//			//�s���ł�����
		//			if (m_actionFlag)
		//			{
		//				//�u���ĂȂ��싞���v��j��
		//				DeleteGO(padlock);
		//				//���g�̃I�u�W�F�N�g��j��
		//				DeleteGO(this);
		//				//�s���ł��Ȃ�����
		//				m_actionFlag = false;
		//			}
		//		}
		//		return true;
		//	}
		//);
		//��Q�I�u�W�F�N�g�́u���v���N�G��
		QueryLOs<OObox>(EN_OO_TYPE_BOX, [&](OObox* box) -> bool
			{
				//���g�Ɓu���v���Փ˂�����
				if (IsHitObject(*this, *box))
				{
					//�s���ł�����
					if (m_actionFlag)
					{
						//�u���v��j��
						DeleteGO(box);
						//���g�̃I�u�W�F�N�g��j��
						DeleteGO(this);
						//�s���ł��Ȃ��悤�ɂ���
						m_actionFlag = false;

						//����Ƃ��̃T�E���h�̍Đ�
						m_brokenSE->Play(false);

						//�A�b�v�x�N�g��
						Vector3 upVec = g_VEC3_UP;
						//�A�b�v�x�N�g�������g�̉�]�ŉ�
						m_rotation.Apply(upVec);
						//���̒l��ύX���č����𒲐߂���
						const float upVecLne = 100.0f;
						upVec.Scale(upVecLne);
						//�G�t�F�N�g�̍��W����]�𒲐߂���
						m_effect->SetPosition(m_position + upVec);		//���W��n��
						m_effect->SetRotation(m_rotation);
						//�G�t�F�N�g�̍Đ�
						m_effect->Play();

					}
				}
				return true;
			}
		);

	}
}