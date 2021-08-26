#pragma once
#include "ObstacleObject.h"
#include "SpriteRender.h"
#include "effect/Effect.h"
/**
 * @brief ���r�����X
*/
namespace nsMobyrinth
{
	/**
	 * @brief ���x���I�u�W�F�N�g
	*/
	namespace nsLevelObject
	{
		/**
		 * @brief ��Q�I�u�W�F�N�g
		*/
		namespace nsObstacleObject
		{

			/**
			 * @brief �S�[���̒萔�f�[�^
			*/
			namespace goalConstData
			{
				/*
				* �G�t�F�N�g�֘A�̒萔
				*/
				//�S�[�����̃G�t�F�N�g�̃t�@�C���p�X
				constexpr const char16_t* const EFFECT_FILEPAHT_GOAL = u"Assets/effect2/goal.efk";
				//�S�[�����̃G�t�F�N�g�̊g�嗦
				const Vector3 EFFECT_SCALE_GOAL = { 150.0f,150.0f,150.0f };

				/*
				* �X�v���C�g�֘A�̒萔
				*/

				//�S�[�����̃X�v���C�g�̃t�@�C���p�X
				constexpr const char* const SPRITE_FILEPATH_GOAL = "Assets/Image/Clear.DDS";
				//�S�[�����̃X�v���C�g�̕�
				constexpr float SPRITE_WIDHT_GOAL = 1024.0f;
				//�S�[�����̃X�v���C�g�̍���
				constexpr float SPRITE_HEIGHT_GOAL = 512.0f;
				//�S�[�����̃X�v���C�g�̍��W
				const Vector3 SPRITE_POSITION_GOAL = { 0.0f,250.0f,0.0f };

				/*
				* �T�E���h�֘A�̒萔
				*/

				//�S�[����SE�̃t�@�C���p�X
				constexpr const wchar_t* const SOUND_FILEPATH_SE_GOAL = L"Assets/sound/goal.wav";
				//�S�[����SE�̉���
				constexpr float SOUND_VOLUME_SE_GOAL = 0.5f;
			}

			/// <summary>
			/// ��Q�I�u�W�F�N�g
			/// �S�[���I�u�W�F�N�g
			/// </summary>
			class OOgoal : public CObstacleObject
			{
			public:		//�����ŌĂ΂�郁���o�֐�
				bool StartSub()override final;	//�X�^�[�g�֐�
				~OOgoal();						//�f�X�g���N�^
				void UpdateSub()override final;	//�A�b�v�f�[�g�֐�

			public:		//�����o�֐�

				/// <summary>
				/// �S�[�����Ă��邩�ǂ����𓾂�
				/// </summary>
				/// <returns>�S�[�����Ă��邩�H</returns>
				bool GetIsGoal()const
				{
					return m_isGoal;
				}

			private:	//private�ȃ����o�֐�

				/**
				 * @brief �G�t�F�N�g�̏���������
				*/
				void InitEffect();

				/**
				 * @brief �X�v���C�g�̏���������
				*/
				void InitSprite();

				/**
				 * @brief �T�E���h�̏���������
				*/
				void InitSound();

				/**
				 * @brief �S�[�����̏���
				*/
				void Goal();

			private:	//�f�[�^�����o
				bool m_isGoal = false;				//�S�[������
				Effect* m_goalEF = nullptr;			//�S�[�����̃G�t�F�N�g
				nsGraphic::nsSprite::CSpriteRender* m_goalSR = nullptr;	//�S�[�����̃X�v���C�g
				nsSound::CSoundCue* m_goalSC = nullptr;		//�S�[�����̃T�E���h�L���[
			};

		}
	}
}