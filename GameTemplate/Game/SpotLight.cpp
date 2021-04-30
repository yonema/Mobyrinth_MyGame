#include "stdafx.h"
#include "SpotLight.h"


//�R���X�g���N�^
CSpotLight::CSpotLight()
{
	m_result = CLightManager::GetInstance()->AddLight(this);
	if (m_result == false)
	{
		DeleteGO(this);
	}
}

//�f�X�g���N�^
CSpotLight::~CSpotLight()
{
	if (m_result)
		CLightManager::GetInstance()->RemoveLight(this);
}

/// <summary>
/// �X�|�b�g���C�g�̍��W��ݒ�
/// </summary>
/// <param name="direction">���C�g�̍��W</param>
void CSpotLight::SetPosition(const Vector3& position)
{
	if (m_result)
		m_light->position = position;
}

/// <summary>
/// �X�|�b�g���C�g�̐F�̐ݒ�
/// </summary>
/// <param name="color">���C�g�̐F</param>
void CSpotLight::SetColor(const Vector4& color)
{
	if (m_result)
		m_light->color = color;
}

/// <summary>
/// �X�|�b�g���C�g�̉e���͈͂�ݒ�
/// </summary>
/// <param name="renge">�e���͈�</param>
void CSpotLight::SetRange(const float range)
{
	if (m_result)
		m_light->range = range;
}