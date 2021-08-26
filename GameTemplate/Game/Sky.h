#pragma once
#include "SkyConstData.h"

/**
 * @brief ���r�����X
*/
namespace nsMobyrinth
{
	/**
	 * @brief �X�J�C
	*/
	namespace nsSky
	{
		/**
		 * @brief ��N���X
		*/
		class CSky : public IGameObject
		{
		public:		//�����ŌĂ΂�郁���o�֐�
			CSky();		//�R���X�g���N�^
			~CSky();	//�f�X�g���N�^
			bool Start()override final;		//�X�^�[�g�֐�
			void Update()override final;	//�A�b�v�f�[�g�֐�
			void ForwardRender(RenderContext& rc)override final;	//�`��֐�

		public:		//�����o�֐�

			/**
			 * @brief ���W��ݒ�
			 * @param [in] pos ���W
			*/
			void SetPosition(const Vector3& pos)
			{
				m_position = pos;
				//�l���ς��������A�X�V���K�v
				m_isDirty = true;
			}

			/**
			 * @brief �g�嗦��ݒ�iVector3�j
			 * @param [in] scale �g�嗦
			*/
			void SetScale(const Vector3& scale)
			{
				m_scale = scale;
				//�l���ς��������A�X�V���K�v
				m_isDirty = true;
			}

			/**
			 * @brief �g�嗦��ݒ�ifloat�j
			 * @param [in] scale �g�嗦
			*/
			void SetScale(const float scale)
			{
				//�g�嗦�����������Ă���
				m_scale = g_VEC3_ONE;
				//�g��
				m_scale.Scale(scale);
				//�l���ς��������A�X�V���K�v
				m_isDirty = true;
			}

			/**
			 * @brief ���Ȕ����J���[��ݒ�
			 * @param [in] color ���Ȕ�������\
			*/
			void SetModelEmissionColor(const Vector4 color)
			{
				m_model.SetModelEmissionColor(color);
				//�l���ς��������A�X�V��
				m_isDirty = true;
			}

			/**
			 * @brief �X�J�C�L���[�u�e�N�X�`���̃t�@�C���p�X��ݒ�
			 * @attention Start�֐���葁���Ă΂Ȃ��ƌ��ʂ��Ȃ�
			 * @param filepath �X�J�C�L���[�u�e�N�X�`���̃t�@�C���p�X
			*/

			/**
			 * @brief ��̎�ނ�ݒ�
			 * @attention Start�֐���葁���Ă΂Ȃ��ƌ��ʂ��Ȃ�
			 * @param skyType ��̎��
			*/
			void SetSkyType(const skyConstData::EN_SKY_TYPE skyType)
			{
				m_skyType = skyType;
			}

		private:	//�f�[�^�����o
			Model m_model;							//���f���N���X
			Texture m_skyCubeTexture;				//�X�J�C�L���[�u�e�N�X�`��
			Vector3 m_position = g_VEC3_ZERO;		//���W
			Vector3 m_scale = g_VEC3_ONE;			//�g�嗦
			Vector4 m_selfLuminous = g_VEC4_BLACK;	//���Ȕ����F
			bool m_isDirty = false;					//�_�[�e�B�t���O
			skyConstData::EN_SKY_TYPE m_skyType = skyConstData::EN_SKY_DEFAULT;	//��̎��
		};

	}
}