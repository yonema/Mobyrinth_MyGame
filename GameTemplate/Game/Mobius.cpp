#include "stdafx.h"
#include "Mobius.h"
#include "LevelObjectManager.h"


//�X�^�[�g�֐�
bool Mobius::Start()
{
	//���f�������_���[�̐���
	m_modelRender = NewGO<CModelRender>(0);
	//���f�������_���[�̏�����
	m_modelRender->Init("Assets/modelData/Mobius.tkm");
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
	m_modelRender->SetShadowCasterFlag(true);

	return true;
}

//�f�X�g���N�^
Mobius::~Mobius()
{
	//���f�������_���[�̔j��
	DeleteGO(m_modelRender);
	//�}�l�[�W���[�Ƀ��r�E�X�̗ւ��������ꂽ�Ɠ`����
	CLevelObjectManager::GetInstance()->SetMobius(nullptr);
}

