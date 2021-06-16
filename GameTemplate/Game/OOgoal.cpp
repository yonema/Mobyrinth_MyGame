#include "stdafx.h"
#include "OOgoal.h"

//�X�^�[�g�֐�
bool OOgoal::StartSub()
{
	//�������p�֐�
	Init("Assets/modelData/goal.tkm", EN_OO_TYPE_GOAL);

	//�t�H���g�����_���[�̐���
	//m_spriteRender = NewGO<CSpriteRender>(2);
	//������
	//m_spriteRender->Init
	//("Assets/Image/Clear.DDS", 1024.0f, 512.0f, {0.5f,0.5f}, AlphaBlendMode_Trans);

	//����������
	//m_spriteRender->Deactivate();

	m_effect = NewGO<Effect>(0);
	m_effect->Init(u"Assets/effect/wing.efk");
	m_effect->SetPosition(m_position);
	m_effect->SetRotation(m_rotation);
	const float scale = 50.0f;
	m_effect->SetScale({ scale ,scale ,scale });
	//m_effect->Play();


	//OBBWorld�Ɏ��g��OBB�̓o�^������������
	COBBWorld::GetInstance()->RemoveOBB(&GetOBB());

	return true;
}

//�f�X�g���N�^
OOgoal::~OOgoal()
{
	//DeleteGO(m_spriteRender);
}

//�A�b�v�f�[�g�֐�
void OOgoal::UpdateSub()
{
	//�q�b�g�t���O���܂��Ă��邩�H
	if (!GetIsHitFlag())
	{
		//�܂��Ă���
		
		m_effect->Deactivate();
		return;
	}

	if (!m_effect->IsActive())
	{
		m_effect->Activate();
	}
	const float m_rePlayTime = 1.5;
	if (m_timer >= m_rePlayTime)
	{
		//m_effect->Play();

		m_timer = 0.0f;
	}
	else
	{
		m_timer += GameTime().GetFrameDeltaTime();
	}

	//���������H
	if (m_player->GetFallFalg())
		//�������̓S�[����������Ȃ�
		return;

	//���g�ƃv���C���[���Փ˂�����
	if (IsHitPlayer())
	{
		//�S�[����Ԃɂ���
		m_isGoal = true;
		//�t�H���g�����_���[��L��������
		//m_spriteRender->Activate();
		//�v���C���[���S�[����Ԃɂ���
		m_player->SetGoal(true);
		////���f���������Ȃ�����
		//GetModelRender()->Deactivate();
		DeleteGO(this);
	}

}

