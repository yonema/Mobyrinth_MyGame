#include "stdafx.h"
#include "PointLight.h"

/**
 * @brief メビリンス
*/
namespace nsMobyrinth
{
	/**
	 * @brief ライト
	*/
	namespace nsLight
	{

		//コンストラクタ
		CPointLight::CPointLight()
		{
			m_result = CLightManager::GetInstance()->AddLight(this);
			if (m_result == false)
			{
				DeleteGO(this);
			}
		}

		//デストラクタ
		CPointLight::~CPointLight()
		{
			if (m_result)
				CLightManager::GetInstance()->RemoveLight(this);
		}

		/// <summary>
		/// ポイントライトの座標を設定
		/// </summary>
		/// <param name="direction">ライトの座標</param>
		void CPointLight::SetPosition(const Vector3& position)
		{
			if (m_result)
				m_light->ptPosition = position;
		}

		/// <summary>
		/// ポイントライトの色の設定
		/// </summary>
		/// <param name="color">ライトの色</param>
		void CPointLight::SetColor(const Vector4& color)
		{
			if (m_result)
				m_light->ptColor = color;
		}

		/// <summary>
		/// ポイントライトの影響範囲を設定
		/// </summary>
		/// <param name="renge">影響範囲</param>
		void CPointLight::SetRange(const float range)
		{
			if (m_result)
				m_light->ptRange = range;
		}

	}
}