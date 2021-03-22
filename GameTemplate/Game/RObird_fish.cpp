#include "stdafx.h"
#include "RObird_fish.h"

//�X�^�[�g�֐�
bool RObird_fish::StartSub()
{
	//�������p�֐�
	Init("Assets/modelData/bird.tkm", enBird,
		"Assets/modelData/fish.tkm", enFish);

	//�\�Ɨ��̃��f�������_���[�̐���
	for (int i = 0; i < enFrontAndBackNum; i++)
	{
		m_otherModelRender[i] = NewGO<CModelRender>(0);
	}

	//������̕\�Ɨ��̃��f�������_���[�̏�����
	m_otherModelRender[enFront]->Init("Assets/modelData/grilled chicken.tkm");
	m_otherModelRender[enBack]->Init("Assets/modelData/grilled fish.tkm");
	//������̕\�Ɨ��̃I�u�W�F�N�g�̃^�C�v��ݒ�
	m_reversibleType[enFront] = enGrilledChicken;
	m_reversibleType[enBack] = enGrilledFish;

	//���f�������_���[�𖳌������Ĕ�펞�ɂ���
	for (int i = 0; i < enFrontAndBackNum; i++)
	{
		m_otherModelRender[i]->Deactivate();
	}

	return true;
}

//�f�X�g���N�^
RObird_fish::~RObird_fish()
{
	//�\�Ɨ��̃��f�������_���[��j��
	for (int i = 0; i < enFrontAndBackNum; i++)
	{
		DeleteGO(m_otherModelRender[i]);
	}
}

/// <summary>
/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐�
/// </summary>
void RObird_fish::QuerySub()
{
	//���g���u���v�̎�
	if (GetObjectType() == enBird)
	{
		//��Q�I�u�W�F�N�g�́u�ł����΁v���N�G��
		QueryLOs<OObigFire>(enBigFire, [&](OObigFire* bigFire) -> bool
			{
				//���g�Ɓu�ł����΁v���Փ˂�����
				if (IsHitObject(*this, *bigFire))
				{
					//���̗��\�̃��f���𖳌�������
					SetBothModelActiveFlag(false);
					//������̗��\�̃��f����L��������
					m_otherModelRender[GetFrontOrBack()]->Activate();
				}
				//true��߂�
				return true;
			}
		);
	}
	//���g���u���v�̎�
	else if (GetObjectType() == enFish)
	{
		//��Q�I�u�W�F�N�g�́u�ł����΁v���N�G��
		QueryLOs<OObigFire>(enBigFire, [&](OObigFire* bigFire) -> bool
			{
				//���g�Ɓu�ł����΁v���Փ˂�����
				if (IsHitObject(*this, *bigFire))
				{
					//���̗��\�̃��f���𖳌�������
					SetBothModelActiveFlag(false);
					//������̗��\�̃��f����L��������
					m_otherModelRender[GetFrontOrBack()]->Activate();
				}
				//true��߂�
				return true;
			}
		);
	}
}

//�A�b�v�f�[�g�֐�
void RObird_fish::UpdateSub()
{
	//������̗��\���f���̍X�V
	for (int i = 0; i < enFrontAndBackNum; i++)
	{
		//������̗��\���f���̏ꏊ�Ɖ�]�Ɗg���ݒ�
		m_otherModelRender[i]->SetPosition(m_position);
		m_otherModelRender[i]->SetRotation(m_rotation);
		m_otherModelRender[i]->SetScale(m_scale);
	}
}

//�\�Ɨ����Z�b�g����T�u�֐�
void RObird_fish::SetFrontOrBackSub()
{
	//������̗��\�̃^�C�v��ݒ肷��
	SetObjectType(m_reversibleType[GetFrontOrBack()]);
	//�\�������H���\�Ȃ�
	if (m_otherModelRender[GetFrontOrBack()])
		//�\��L�������ĕ\������
		m_otherModelRender[GetFrontOrBack()]->Activate();

	//�\�������H�����Ȃ�
	if (m_otherModelRender[!GetFrontOrBack()])
		//����L�������ĕ\������
		m_otherModelRender[!GetFrontOrBack()]->Deactivate();
}