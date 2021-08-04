#include "stdafx.h"
#include "LightManager.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"

//static�ȃf�[�^�����o�̏�����
CLightManager* CLightManager::m_instance = nullptr;
SLightParam CLightManager::m_lightParam;

//�R���X�g���N�^
CLightManager::CLightManager()
{
	if (m_instance != nullptr) {
		//�C���X�^���X�����łɍ���Ă���B
		std::abort();
	}
	Init();	//����������
}

//�f�X�g���N�^
CLightManager::~CLightManager()
{
	//nullptr�����Ă���
	m_instance = nullptr;
}

//�������֐�
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


//�A�b�v�f�[�g
void CLightManager::Update()
{
	//���_���J�����̈ʒu�ɂ���
	m_lightParam.eyePos = g_camera3D->GetPosition();
}

/// <summary>
/// ���C�g��ǉ�����֐��i�f�B���N�V�������C�g�j
/// </summary>
/// <param name="light">�ǉ����郉�C�g</param>
/// <returns>�ǉ��ł�����true��߂�</returns>
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

/// <summary>
/// ���C�g��ǉ�����֐��i�|�C���g���C�g�j
/// </summary>
/// <param name="light">�ǉ����郉�C�g</param>
/// <returns>�ǉ��ł�����true��߂�</returns>
bool CLightManager::AddLight(CPointLight* light)
{

	//�|�C���g���C�g�̐���MAX��葽�����������Ȃ�
	if (m_lightParam.numPointLight >= Max_PointLight) {
		return false;
	}

	//Manager�Ƀ|�C���g���C�g�̎Q�Ƃ�n��
	m_pointLights[m_lightParam.numPointLight]
		= light;
	//Manager�̃|�C���g���C�g�̏���������ݒ�
	m_pointLightsData[m_lightParam.numPointLight].ptPosition
		= { 0.0f,0.0f,0.0f };
	m_pointLightsData[m_lightParam.numPointLight].ptColor
		= { 0.0f,0.0f,0.0f,1.0f };
	m_pointLightsData[m_lightParam.numPointLight].ptRange = 300.0f;
	//Light��Manager�̃|�C���g���C�g�̎Q�Ƃ�n��
	light->SetRawData(&m_pointLightsData[m_lightParam.numPointLight]);
	//Light��Manager�̊Ǘ��ԍ���n���B
	light->SetControlNumber(m_lightParam.numPointLight);

	//���݂̃��C�g�̐��𑝂₷
	m_lightParam.numPointLight++;

	return true;
}

/// <summary>
/// ���C�g��ǉ�����֐��i�X�|�b�g���C�g�j
/// </summary>
/// <param name="light">�ǉ����郉�C�g</param>
/// <returns>�ǉ��ł�����true��߂�</returns>
bool CLightManager::AddLight(CSpotLight* light)
{

	//�X�|�b�g���C�g�̐���MAX��葽�����������Ȃ�
	if (m_lightParam.numSpotLight >= Max_SpotLight) {
		return false;
	}

	//Manager�ɃX�|�b�g���C�g�̎Q�Ƃ�n��
	m_spotLights[m_lightParam.numSpotLight]
		= light;
	//Manager�̃X�|�b�g���C�g�̏����l��ݒ�
	m_spotLightsData[m_lightParam.numSpotLight].position
		= { 0.0f,0.0f,0.0f };
	m_spotLightsData[m_lightParam.numSpotLight].color
		= { 0.0f,0.0f,0.0f,1.0f };
	m_spotLightsData[m_lightParam.numSpotLight].range = 300.0f;
	m_spotLightsData[m_lightParam.numSpotLight].direction = g_VEC3_DOWN;
	m_spotLightsData[m_lightParam.numSpotLight].angle = 45.0f;
	//Light��Manager�̃X�|�b�g���C�g�̎Q�Ƃ�n��
	light->SetRawData(&m_spotLightsData[m_lightParam.numSpotLight]);
	//Light��Manager�̊Ǘ��ԍ���n���B
	light->SetControlNumber(m_lightParam.numSpotLight);

	//���݂̃��C�g�̐��𑝂₷
	m_lightParam.numSpotLight++;

	return true;
}

/// <summary>
/// ���C�g����������֐��i�f�B���N�V�������C�g�j
/// </summary>
/// <param name="light">�������郉�C�g</param>
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

/// <summary>
/// ���C�g����������֐��i�|�C���g���C�g�j
/// </summary>
/// <param name="light">�������郉�C�g</param>
void CLightManager::RemoveLight(CPointLight* light)
{
	//�������C�g�̊Ǘ��ԍ����擾
	const int targetLigNumBuff = light->GetControlNumver();

	//i�̈�悪�|�C���g���C�g�̐���菬�����Ƃ��p��
	for (int i = targetLigNumBuff; i + 1 < m_lightParam.numPointLight; i++)
	{
		//Manager�́A�|�C���g���C�g������
		//��������́ALight�̊Ǘ��ԍ��Ƃ��̈��̔ԍ�
		std::swap<SPointLight>(m_pointLightsData[i], m_pointLightsData[i + 1]);
		//Manager�́ALight�̎Q�Ƃ�����
		std::swap<CPointLight*>(m_pointLights[i], m_pointLights[i + 1]);
	}

	//i�̈�悪�|�C���g���C�g�̐���菬�����Ƃ��p��
	for (int i = targetLigNumBuff; i + 1 < m_lightParam.numPointLight; i++)
	{
		//Light�̃|�C���g���C�g�̍\���̂�
		m_pointLights[i]->SetRawData(&m_pointLightsData[i]);
	}

	m_lightParam.numPointLight--;

}


/// <summary>
/// ���C�g����������֐��i�X�|�b�g���C�g�j
/// </summary>
/// <param name="light">�������郉�C�g</param>
void CLightManager::RemoveLight(CSpotLight* light)
{
	//�������C�g�̊Ǘ��ԍ����擾
	const int targetLigNumBuff = light->GetControlNumver();

	//i�̈�悪�X�|�b�g���C�g�̐���菬�����Ƃ��p��
	for (int i = targetLigNumBuff; i + 1 < m_lightParam.numSpotLight; i++)
	{
		//Manager�́A�X�|�b�g���C�g������
		//��������́ALight�̊Ǘ��ԍ��Ƃ��̈��̔ԍ�
		std::swap<SSpotLight>(m_spotLightsData[i], m_spotLightsData[i + 1]);
		//Manager�́ALight�̎Q�Ƃ�����
		std::swap<CSpotLight*>(m_spotLights[i], m_spotLights[i + 1]);
	}

	//i�̈�悪�X�|�b�g���C�g�̐���菬�����Ƃ��p��
	for (int i = targetLigNumBuff; i + 1 < m_lightParam.numSpotLight; i++)
	{
		//Light�̃X�|�b�g���C�g�̍\���̂�
		m_spotLights[i]->SetRawData(&m_spotLightsData[i]);
	}

	m_lightParam.numSpotLight--;

}