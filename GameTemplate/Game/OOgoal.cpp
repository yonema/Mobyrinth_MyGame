#include "stdafx.h"
#include "OOgoal.h"

//�X�^�[�g�֐�
bool OOgoal::StartSub()
{
	//�������p�֐�
	Init("Assets/modelData/goal.tkm", enGoal);

	//�t�H���g�����_���[�̐���
	m_spriteRender = NewGO<CSpriteRender>(2);
	//������
	m_spriteRender->Init
	("Assets/Image/Clear.DDS", 1024.0f, 512.0f, {0.5f,0.5f}, AlphaBlendMode_Trans);

	//����������
	m_spriteRender->Deactivate();

	//OBBWorld�Ɏ��g��OBB�̓o�^������������
	COBBWorld::GetInstance()->RemoveOBB(&GetOBB());

	m_effect = NewGO<Effect>(0);
	m_effect->Init(u"Assets/effect/ster.efk");
	float scale = 50.0f;
	m_effect->SetScale({ scale ,scale ,scale });
	m_effect->SetPosition(m_position);
	m_effect->Play();


	return true;
}

//�f�X�g���N�^
OOgoal::~OOgoal()
{
	DeleteGO(m_spriteRender);
	DeleteGO(m_effect);
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
		m_spriteRender->Activate();
	}

}

