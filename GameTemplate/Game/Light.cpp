#include "stdafx.h"
#include "Light.h"

Light* Light::m_instance = nullptr;

Light::Light()
{
	if (m_instance != nullptr) {
		//�C���X�^���X�����łɍ���Ă���B
		std::abort();
	}
	Init();
	m_instance = this;
}

Light::~Light()
{
	m_instance = nullptr;
}

void Light::Init()
{
	//�f�B���N�V�������C�g
	m_directionLight.dirLigDirection.x = 1.0f;
	m_directionLight.dirLigDirection.y = -1.0f;
	m_directionLight.dirLigDirection.z = -1.0f;
	m_directionLight.dirLigDirection.Normalize();			//����
	m_directionLight.dirLigColor = { 1.0f,0.4f,0.4f };	//�F
	m_directionLight.eyePos = g_camera3D->GetPosition();	//���_
	m_directionLight.specPow = 5.0f;						//�X�y�L�����̍i��
	m_directionLight.ambinetLight = { 0.6f,0.6f,0.6f };	//����

	//�|�C���g���C�g
	m_pointLight.ptPosition = { 10.0f,10.0f,0.0f };
	m_pointLight.ptRange = 100.0f;
	m_pointLight.ptColor = { 1.0f,1.0f,0.0f };
}
