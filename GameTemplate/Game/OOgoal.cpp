#include "stdafx.h"
#include "OOgoal.h"

//�X�^�[�g�֐�
bool OOgoal::StartSub()
{
	//�������p�֐�
	Init("Assets/modelData/goal.tkm", enGoal);

	//�t�H���g�����_���[�̐���
	m_fontRender = NewGO<CFontRender>(0);
	//������
	m_fontRender->Init(L"�S�[���I",
		{-200.0f,100.0f},
		{1.0f,1.0f,0.0f,1.0f},
		0.0f,
		3.0f
		);
	//�|�X�g�����_���[�ŕ`�悷��
	m_fontRender->SetPostRenderFlag(true);
	//����������
	m_fontRender->Deactivate();

	//OBBWorld�Ɏ��g��OBB�̓o�^������������
	COBBWorld::GetInstance()->RemoveOBB(&GetOBB());

	return true;
}

//�f�X�g���N�^
OOgoal::~OOgoal()
{
	DeleteGO(m_fontRender);
}

//�A�b�v�f�[�g�֐�
void OOgoal::UpdateSub()
{
	//���g�ƃv���C���[���Փ˂�����
	if (IsHitPlayer())
	{
		//�S�[����Ԃɂ���
		m_isGoal = true;
		//�t�H���g�����_���[��L��������
		m_fontRender->Activate();
	}

}

