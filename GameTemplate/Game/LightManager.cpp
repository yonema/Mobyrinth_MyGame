#include "stdafx.h"
#include "LightManager.h"
#include "DirectionLight.h"

//static�ȃf�[�^�����o�̏�����
CLightManager* CLightManager::m_instance = nullptr;
SLightParam CLightManager::m_lightParam;

CLightManager::CLightManager()
{
	if (m_instance != nullptr) {
		//�C���X�^���X�����łɍ���Ă���B
		std::abort();
	}
	Init();	//����������
}

CLightManager::~CLightManager()
{
	//nullptr�����Ă���
	m_instance = nullptr;
}

void CLightManager::Init()
{
	//���C�g�̋��ʂ̃p�����[�^�̏����l��ݒ�
	m_lightParam.eyePos = { 0.0f,0.0f,0.0f };
	m_lightParam.numDirectionLight = 0;
	m_lightParam.ambientLight = { 0.6f,0.6f,0.6f };
	m_lightParam.numPointLight = 0;
	m_lightParam.specPow = 5.0f;
	m_lightParam.numShadow = 0;
}



void CLightManager::ExecuteUpdate()
{
	//���_���J�����̈ʒu�ɂ���
	m_lightParam.eyePos = g_camera3D->GetPosition();
}
bool CLightManager::AddLight(CDirectionLight* light)
{

	//�f�B���N�V�������C�g�̐���MAX��葽�����������Ȃ�
	if (m_lightParam.numDirectionLight >= Max_DirectionLight) {
		//TK_WARNING("m_directionLights.size() is full!!");
		return false;
	}

		//Manager�Ƀf�B���N�V�������C�g�̎Q�Ƃ�n��
		m_directionLights[m_lightParam.numDirectionLight]
			= light;
		//Manager�̃f�B���N�V�������C�g�̏���������ݒ�
		m_directionLightsData[m_lightParam.numDirectionLight].dirLigDirection
			= { 0.0f,0.0f,0.0f };
		m_directionLightsData[m_lightParam.numDirectionLight].dirLigColor
			= { 0.0f,0.0f,0.0f,1.0f };
		//Light��Manager�̃f�B���N�V�������C�g�̎Q�Ƃ�n��
		light->SetRawData(&m_directionLightsData[m_lightParam.numDirectionLight]);
		//Light��Manager�̊Ǘ��ԍ���n���B
		light->SetControlNumber(m_lightParam.numDirectionLight);
		
		//���݂̃��C�g�̐��𑝂₷
		m_lightParam.numDirectionLight++;
	
		return true;
}


void CLightManager::RemoveLight(CDirectionLight* light)
{
	//�������C�g�̊Ǘ��ԍ����擾
	const int targetLigNumBuff = light->GetControlNumver();
		
	//i�̈�悪�f�B���N�V�������C�g�̐���菬�����Ƃ��p��
	for (int i = targetLigNumBuff; i + 1 < m_lightParam.numDirectionLight; i++)
	{
		//Manager�́A�f�B���N�V�������C�g������
		//��������́ALight�̊Ǘ��ԍ��Ƃ��̈��̔ԍ�
		std::swap<SDirectionLight>(m_directionLightsData[i], m_directionLightsData[i + 1]);
		//Manager�́ALight�̎Q�Ƃ�����
		std::swap<CDirectionLight*>(m_directionLights[i], m_directionLights[i + 1]);
	}

	//i�̈�悪�f�B���N�V�������C�g�̐���菬�����Ƃ��p��
	for (int i = targetLigNumBuff; i + 1 < m_lightParam.numDirectionLight; i++)
	{
		//Light�̃f�B���N�V�������C�g�̍\���̂�
		m_directionLights[i]->SetRawData(&m_directionLightsData[i]);
	}

	m_lightParam.numDirectionLight--;


}