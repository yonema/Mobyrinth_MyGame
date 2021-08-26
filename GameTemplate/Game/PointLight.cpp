#include "stdafx.h"
#include "PointLight.h"

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
		CPointLight::CPointLight()
		{
			m_result = CLightManager::GetInstance()->AddLight(this);
			if (m_result == false)
			{
				DeleteGO(this);
			}
		}

		//�f�X�g���N�^
		CPointLight::~CPointLight()
		{
			if (m_result)
				CLightManager::GetInstance()->RemoveLight(this);
		}

		/// <summary>
		/// �|�C���g���C�g�̍��W��ݒ�
		/// </summary>
		/// <param name="direction">���C�g�̍��W</param>
		void CPointLight::SetPosition(const Vector3& position)
		{
			if (m_result)
				m_light->ptPosition = position;
		}

		/// <summary>
		/// �|�C���g���C�g�̐F�̐ݒ�
		/// </summary>
		/// <param name="color">���C�g�̐F</param>
		void CPointLight::SetColor(const Vector4& color)
		{
			if (m_result)
				m_light->ptColor = color;
		}

		/// <summary>
		/// �|�C���g���C�g�̉e���͈͂�ݒ�
		/// </summary>
		/// <param name="renge">�e���͈�</param>
		void CPointLight::SetRange(const float range)
		{
			if (m_result)
				m_light->ptRange = range;
		}

	}
}