#pragma once
#include "ObstacleObject.h"
#include "ReversibleObject.h"
#include "FontRender.h"
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
			 * @brief �u�S���]�v�̒萔�f�[�^
			*/
			namespace reverseAllConstData
			{
				//�A�b�v�f�[�g�X�e�[�g
				enum EN_UPDATE_STATE
				{
					EN_BEFOR_HIT_PLAYER,	//�v���C���[�ƏՓˑO�̏��
					EN_HIT_PLAYER,		//�v���C���[�ƏՓˎ��̏��
					EN_AFTER_HIT_PLAYER,	//�v���C���[�ƏՓˌ�̏��
				};

				//���f���̎��Ȕ����J���[
				const Vector4 MODEL_COLOR_EMISSION = { 0.5f,0.0f,0.5f,1.0f };

				/*
				* �T�E���h�֘A�̒萔�f�[�^
				*/

				//�S���]�̎��̃T�E���h�̃t�@�C���p�X
				constexpr const wchar_t* const SOUND_FILEPATH_CHANGE_ALL = L"Assets/sound/allchange.wav";
				//�S���]�̎��̃T�E���h�̉���
				constexpr float SOUND_VALUME_CHANGE_ALL = 1.0f;

				/*
				* �G�t�F�N�g�֘A�̒萔�f�[�^
				*/

				//�S���]���g��\���G�t�F�N�g�̃t�@�C���p�X
				constexpr const char16_t* const EFFECT_FILEPATH_REVERSE_ALL = u"Assets/effect/reverseall.efk";
				//�S���]���g��\���G�t�F�N�g�̊g�嗦
				const Vector3 EFFECT_SCALE_REVERSE_ALL = { 200.0f,200.0f,200.0f };
				//�S���]���g��\���G�t�F�N�g�̃��[�v�؂�ւ�����
				constexpr float EFFECT_TIME_LOOP = 2.0f;

				//�S���]���鎞�̃G�t�F�N�g�̃t�@�C���p�X
				constexpr const char16_t* const EFFECT_FILEPATH_CHANGE_ALL = u"Assets/effect/reverseall2.efk";
				//�S���]���鎞�̃G�t�F�N�g�̊g�嗦
				const Vector3 EFFECT_SCALE_CHANGE_ALL = { 50.0f,50.0f,50.0f };
				//�S���]���鎞�̃G�t�F�N�g�̏�ɂ����钷��
				constexpr float EFFECT_LENGHT_POSITION_CHANGE_ALL = 100.0f;
			}

			/// <summary>
			/// ��Q�I�u�W�F�N�g
			/// �S���]�I�u�W�F�N�g
			/// </summary>
			class OOReverseALL : public CObstacleObject
			{
			public:		//�����ŌĂ΂�郁���o�֐�
				bool StartSub() override final;		//�X�^�[�g�֐�
				~OOReverseALL();					//�f�X�g���N�^
				void UpdateSub() override final;	//�A�b�v�f�[�g�֐�
				void AlwaysUpdate()override final;	//�|�[�Y���ł����ł��A�b�v�f�[�g����֐�

			private:	//private�ȃ����o�֐�

				/**
				 * @brief �T�E���h�̏���������
				*/
				void InitSound();

				/**
				 * @brief �G�t�F�N�g�̏���������
				*/
				void InitEffect();

				void BeforeHitPlayer();	//�v���C���[�ƏՓˑO�̏���

				void HitPlayer();		//�v���C���[�ƏՓˎ��̏���

				void AfterHitPlayer();	//�v���C���[�ƏՓˌ�̏���

			private:	//�f�[�^�����o

				//�A�b�v�f�[�g�X�e�[�g
				reverseAllConstData::EN_UPDATE_STATE m_updateState = reverseAllConstData::EN_BEFOR_HIT_PLAYER;
				Vector3 m_playerBeforePosition = g_VEC3_ZERO;	//�ՓˑO�̃v���C���[�̍��W
				Vector3 m_playerHitPosition = g_VEC3_ZERO;		//�Փˎ��̃v���C���[�̍��W
				Vector3 m_playerAfterPosition = g_VEC3_ZERO;	//�Փˌ�̃v���C���[�̍��W
				float m_effectLoopTimer = 0.0f;		//�G�t�F�N�g�����[�v�����邽�߂̃^�C�}�[
				Effect* m_reverslAllEF = nullptr;	//�S���]���g��\���G�t�F�N�g
				Effect* m_changeAllEF = nullptr;	//�S���]����Ƃ��̃G�t�F�N�g
				nsSound::CSoundCue* m_changeAllSC = nullptr; //�S���]���鎞�̃T�E���h

			private:	//�f�o�b�N�p

#ifdef MY_DEBUG
//��ŏ���
				static const int m_vertNum = 8;		//���_�̐�
				CModelRender* m_dbgVertPosMR[m_vertNum] = { nullptr };	//���_�����邽�߂̃��f��
#endif
			};

		}
	}
}