#include "stdafx.h"
#include "DirectionLight.h"

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
		CDirectionLight::CDirectionLight()
		{
			m_result = CLightManager::GetInstance()->AddLight(this);
			if (m_result == false)
			{
				DeleteGO(this);
			}
		}

		//デストラクタ
		CDirectionLight::~CDirectionLight()
		{
			if (m_result)
				CLightManager::GetInstance()->RemoveLight(this);
		}


		/// <summary>
		/// ディレクションライトの方向を設定
		/// </summary>
		/// <param name="direction">ライトの方向</param>
		void CDirectionLight::SetDirection(const Vector3& direction)
		{
			if (m_result)
				m_light->dirLigDirection = direction;
		}

		/// <summary>
		/// ディレクションライトの色の設定
		/// </summary>
		/// <param name="color">ライトの色</param>
		void CDirectionLight::SetColor(const Vector4& color)
		{
			if (m_result)
				m_light->dirLigColor = color;
		}
	}
}