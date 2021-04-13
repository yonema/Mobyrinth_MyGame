#include "stdafx.h"
#include "OObigFire.h"


bool OObigFire::StartSub()
{
	//���f���̏������ƃ^�C�v�̐ݒ�
	Init("Assets/modelData/bigFire.tkm", enBigFire);

	//OBB�̃T�C�Y��ݒ�
	Vector3 obbSize;
	obbSize = { 300.0f,300.0f,100.0f };
	GetOBB().SetDirectionLength(obbSize);

	//�|�C���g���C�g�𐶐�
	m_pointLight = NewGO<CPointLight>(0);
	Vector3 ptLigPos = m_position;
	Vector3 up = g_vec3Up;
	m_rotation.Apply(up);
	up.Scale(150.0f);
	ptLigPos += up;
	m_pointLight->SetPosition(m_position);
	m_pointLight->SetColor({ 1000.0f,1.0f,1.0f,1.0f });
	m_pointLight->SetRange(1000.0f);

	//���Ȕ����F��ݒ�
	SetEmissionColor({ 1.0f,0.0f,0.0f,1.0f });

	return true;
}

OObigFire::~OObigFire()
{
	DeleteGO(m_pointLight);
}


void OObigFire::Damage()
{
	if (--m_hp <= 0)
	{
		//HP��0�ȉ���������A���̃I�u�W�F�N�g������
		DeleteGO(this);
	}
	else
	{
		//OBB�̃T�C�Y��HP�ɉ����ď���������
		Vector3 obbSize;
		obbSize = { 300.0f,300.0f,100.0f };
			//m_hp��m_maxHp��int�^�B
			//int�^���m�̊���Z�����珬�����؂�̂Ă���
			//�]�������l��0�ɂȂ��Ă��܂��B
			//������Am_hp��float�ɃL���X�g����
		obbSize.Scale(static_cast<float>(m_hp) / m_maxHp);
		GetOBB().SetDirectionLength(obbSize);

		//���f���̃T�C�Y��HP�ɉ����ď���������
			//���ꂪ��݂̂����ɐ؂�̂ĂɂȂ�Ȃ����R��
			//m_hp / m_maxHp�����g_vec3One * m_hp���Ă΂�
			//Vector3�̉��Z�q�̃I�[�o�[���C�h�̈�����float�^������
			//m_hp���n�����Ƃ��ɋ����^�ϊ����N����float�^�ɂȂ邩��
			//���ꂪSetScale(g_vec3One * (m_hp / m_maxHp))
			//�������炤�܂������Ȃ��B
		SetScale(g_vec3One * m_hp / m_maxHp);
	}
}