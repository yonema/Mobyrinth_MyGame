#pragma once
#include "ReversibleObject.h"
#include "OObox.h"
#include "OOpadlock.h"

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
		 * @brief ���]�I�u�W�F�N�g
		*/
		namespace nsReversibleObject
		{

			/**
			 * @brief �u�B�v�u���Ɓv�̒萔�f�[�^
			*/
			namespace nail_hammerConstData
			{
				//��������Ƃ��̃T�E���h�̃t�@�C���p�X
				constexpr const wchar_t* const SOUNDE_FILEPATH_BREAK_BOX = L"Assets/sound/boxBreak.wav";
				//��������Ƃ��̃T�E���h�̉���
				constexpr float SOUNDE_VOLUME_BREAK_BOX = 0.3f;

				//��������Ƃ��̃G�t�F�N�g�̃t�@�C���p�X
				constexpr const char16_t* const EFFECT_FILEPATH_BREAK_BOX = u"Assets/effect/flame_water.efk";
				//��������Ƃ��̃G�t�F�N�g�̊g�嗦
				const Vector3 EFFECT_SCALE_BREAK_BOX = { 200.0f,200.0f,200.0f };

				//�G�t�F�N�g����ɂ����钷��
				constexpr float LENGTH_UP_POSITION_EFFECT = 100.0f;
			}

			/// <summary>
			/// ���]�I�u�W�F�N�g
			/// �B�A���ƃI�u�W�F�N�g
			/// </summary>
			class ROnail_hammer : public CReversibleObject
			{
			public:		//�����ŌĂ΂�郁���o�֐�
				bool StartSub() override final;	//�X�^�[�g�֐�
				~ROnail_hammer();					//�f�X�g���N�^

				/// <summary>
				/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐�
				/// </summary>
				void QuerySub()override final;

			private:	//private�ȃ����o�֐�

				/**
				 * @brief �T�E���h�̏���������
				*/
				void InitSound();

				/**
				 * @brief �G�t�F�N�g�̏���������
				*/
				void InitEffect();

				/**
				 * @brief ���g���u���Ɓv�ŁA�u���v�ƏՓ˂������̏���
				 * @param [in] box �u���v�̃|�C���^
				*/
				void HammerHitBox(nsObstacleObject::OObox* box);

			private:	//�f�[�^�����o
				//��x�ɕ����̃I�u�W�F�N�g��j�����邱�Ƃ�h�����߂̃t���O
				bool m_actionFlag = true;			//�s���ł��邩�H
				nsSound::CSoundCue* m_brokenSE = nullptr;	//��ꂽ���̃T�E���h
				Effect* m_brokenEF = nullptr;			//��ꂽ���̃G�t�F�N�g
			};

		}
	}
}