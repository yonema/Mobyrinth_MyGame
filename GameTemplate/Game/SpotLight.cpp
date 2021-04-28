#include "stdafx.h"
#include "SpotLight.h"


//コンストラクタ
CSpotLight::CSpotLight()
{
	m_result = CLightManager::GetInstance()->AddLight(this);
	if (m_result == false)
	{
		DeleteGO(this);
	}
}

//デストラクタ
CSpotLight::~CSpotLight()
{
	if (m_result)
		CLightManager::GetInstance()->RemoveLight(this);
}

/// <summary>
/// スポットライトの座標を設定
/// </summary>
/// <param name="direction">ライトの座標</param>
void CSpotLight::SetPosition(const Vector3& position)
{
	if (m_result)
		m_light->position = position;
}

/// <summary>
/// スポットライトの色の設定
/// </summary>
/// <param name="color">ライトの色</param>
void CSpotLight::SetColor(const Vector4& color)
{
	if (m_result)
		m_light->color = color;
}

/// <summary>
/// スポットライトの影響範囲を設定
/// </summary>
/// <param name="renge">影響範囲</param>
void CSpotLight::SetRange(const float range)
{
	if (m_result)
		m_light->range = range;
}