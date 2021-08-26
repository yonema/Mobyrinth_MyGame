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
		/// �|�C���g���C�g�N���X	
		/// </summary>
		class CPointLight : public IGameObject
		{
		public:		//�����ŌĂ΂�郁���o�֐�
			CPointLight();		//�R���X�g���N�^
			~CPointLight();		//�f�X�g���N�^

		public:		//�����o�֐�

			/// <summary>
			/// �|�C���g���C�g�̍��W��ݒ�
			/// </summary>
			/// <param name="direction">���C�g�̍��W</param>
			void SetPosition(const Vector3& position);

			/// <summary>
			/// �|�C���g���C�g�̍��W���擾
			/// </summary>
			/// <returns>���C�g�̍��W</returns>
			const Vector3& GetPosition() const
			{
				return m_light->ptPosition;
			}

			/// <summary>
			/// �|�C���g���C�g�̐F�̐ݒ�
			/// </summary>
			/// <param name="color">���C�g�̐F</param>
			void SetColor(const Vector4& color);

			/// <summary>
			/// �|�C���g���C�g�̉e���͈͂�ݒ�
			/// </summary>
			/// <param name="renge">�e���͈�</param>
			void SetRange(const float range);

			/// <summary>
			/// ���f�[�^���擾
			/// </summary>
			/// <returns>���f�[�^�̎Q��</returns>
			SPointLight* GetRawData()
			{
				return m_light;
			};

			/// <summary>
			/// ���f�[�^��ݒ�
			/// </summary>
			/// <param name="lig">���f�[�^</param>
			void SetRawData(SPointLight* lig)
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
			SPointLight* m_light = nullptr;		//�|�C���g���C�g�̍\����
			int m_controlNumber;				//����i���o�[
			bool m_result;						//���C�g�}�l�[�W���[�ɓo�^���Ă��邩�H
		};

	}
}