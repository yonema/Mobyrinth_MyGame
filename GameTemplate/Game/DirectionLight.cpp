#include "stdafx.h"
#include "DirectionLight.h"

/**
 * @brief ���r�����X
*/
namespace nsMobyrinth
{
	/**
	 * @brief ���C�g
	*/
	namespace nsLight
	{

		//�R���X�g���N�^
		CDirectionLight::CDirectionLight()
		{
			m_result = CLightManager::GetInstance()->AddLight(this);
			if (m_result == false)
			{
				DeleteGO(this);
			}
		}

		//�f�X�g���N�^
		CDirectionLight::~CDirectionLight()
		{
			if (m_result)
				CLightManager::GetInstance()->RemoveLight(this);
		}


		/// <summary>
		/// �f�B���N�V�������C�g�̕�����ݒ�
		/// </summary>
		/// <param name="direction">���C�g�̕���</param>
		void CDirectionLight::SetDirection(const Vector3& direction)
		{
			if (m_result)
				m_light->dirLigDirection = direction;
		}

		/// <summary>
		/// �f�B���N�V�������C�g�̐F�̐ݒ�
		/// </summary>
		/// <param name="color">���C�g�̐F</param>
		void CDirectionLight::SetColor(const Vector4& color)
		{
			if (m_result)
				m_light->dirLigColor = color;
		}
	}
}