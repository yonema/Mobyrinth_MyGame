#pragma once
#include "LightData.h"
#include "LightManager.h"

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

		/// <summary>
		/// �X�|�b�g���C�g�N���X
		/// </summary>
		class CSpotLight : public IGameObject
		{
		public:		//�����ŌĂ΂�郁���o�֐�
			CSpotLight();	//�R���X�g���N�^
			~CSpotLight();	//�f�X�g���N�^

		public:		//�����o�֐�

			/// <summary>
			/// �X�|�b�g���C�g�̍��W��ݒ�
			/// </summary>
			/// <param name="direction">���C�g�̍��W</param>
			void SetPosition(const Vector3& direction);

			/// <summary>
			/// �X�|�b�g���C�g�̍��W���擾
			/// </summary>
			/// <returns>���C�g�̍��W</returns>
			const Vector3& GetPosition() const
			{
				return m_light->position;
			}

			/// <summary>
			/// �X�|�b�g���C�g�̐F�̐ݒ�
			/// </summary>
			/// <param name="color">���C�g�̐F</param>
			void SetColor(const Vector4& color);

			/// <summary>
			/// �|�C���g���C�g�̉e���͈͂�ݒ�
			/// </summary>
			/// <param name="renge">�e���͈�</param>
			void SetRange(const float range);

			/// <summary>
			/// �X�|�b�g���C�g�̎ˏo�����̐ݒ�
			/// </summary>
			/// <param name="direction">�ˏo����</param>
			void SetDirection(const Vector3 direction)
			{
				if (m_result)
				{
					m_light->direction = direction;
				}
			}

			/// <summary>
			/// �X�|�b�g���C�g�̎ˏo�p�x�̐ݒ�
			/// </summary>
			/// <param name="angle">�ˏo�p�x</param>
			void SetAngle(const float angle)
			{
				if (m_result)
					m_light->angle = angle;
			}

			/// <summary>
			/// ���f�[�^���擾
			/// </summary>
			/// <returns>���f�[�^�̎Q��</returns>
			SSpotLight* GetRawData()
			{
				return m_light;
			};

			/// <summary>
			/// ���f�[�^��ݒ�
			/// </summary>
			/// <param name="lig">���f�[�^</param>
			void SetRawData(SSpotLight* lig)
			{
				m_light = lig;
			}

			/// <summary>
			/// ����i���o�[��ݒ�
			/// </summary>
			/// <param name="num">����i���o�[</param>
			void SetControlNumber(const int num)
			{
				m_controlNumber = num;
			}

			/// <summary>
			/// ����i���o�[���擾
			/// </summary>
			/// <returns>����i���o�[</returns>
			const int GetControlNumver()const
			{
				return m_controlNumber;
			}

		private:	//�f�[�^�����o
			SSpotLight* m_light = nullptr;		//�|�C���g���C�g�̍\����
			int m_controlNumber;				//����i���o�[
			bool m_result;						//���C�g�}�l�[�W���[�ɓo�^���Ă��邩�H

		};

	}
}