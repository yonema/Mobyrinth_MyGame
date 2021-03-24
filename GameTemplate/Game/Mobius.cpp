#include "stdafx.h"
#include "Mobius.h"

//�X�^�[�g�֐�
bool Mobius::Start()
{
	//���f�������_���[�̐���
	m_modelRender = NewGO<CModelRender>(0);
	//���f�������_���[�̏�����
	m_modelRender->Init("Assets/modelData/test2.tkm");
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);

	return true;
}

//�f�X�g���N�^
Mobius::~Mobius()
{
	//���f�������_���[�̔j��
	DeleteGO(m_modelRender);
}

