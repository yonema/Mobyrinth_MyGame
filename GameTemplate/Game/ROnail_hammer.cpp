#include "stdafx.h"
#include "ROnail_hammer.h"

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

			//ReversibleObject�̃��f���̃t�@�C���p�X�̒萔�f�[�^���g�p�\�ɂ���
			using namespace ROsFilepathConstdata;
			//�u�B�v�u���Ɓv�̒萔�f�[�^���g�p�\�ɂ���
			using namespace nail_hammerConstData;

			//�X�^�[�g�֐�
			bool ROnail_hammer::StartSub()
			{
				//�������p�֐�
				Init(MODEL_FILEPATH_NAIL, EN_RO_TYPE_NAIL,
					MODEL_FILEPATH_HAMMER, EN_RO_TYPE_BAR);

				//�T�E���h�̏���������
				InitSound();

				//�G�t�F�N�g�̏���������
				InitEffect();

				return true;
			}

			/**
			 * @brief �T�E���h�̏���������
			*/
			void ROnail_hammer::InitSound()
			{
				//����Ƃ��̃T�E���h�̐����Ə�����
				m_brokenSE = NewGO<nsSound::CSoundCue>(nsCommonData::PRIORITY_FIRST);
				m_brokenSE->Init(SOUNDE_FILEPATH_BREAK_BOX);
				m_brokenSE->SetVolume(SOUNDE_VOLUME_BREAK_BOX);

				return;
			}

			/**
			 * @brief �G�t�F�N�g�̏���������
			*/
			void ROnail_hammer::InitEffect()
			{
				//����Ƃ��̃G�t�F�N�g�̐����Ə�����
				m_brokenEF = NewGO<Effect>(nsCommonData::PRIORITY_FIRST);
				m_brokenEF->Init(EFFECT_FILEPATH_BREAK_BOX);
				m_brokenEF->SetScale(EFFECT_SCALE_BREAK_BOX);

				return;
			}

			//�f�X�g���N�^
			ROnail_hammer::~ROnail_hammer()
			{
				DeleteGO(m_brokenSE);
				DeleteGO(m_brokenEF);
			}

			/// <summary>
			/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐�
			/// </summary>
			void ROnail_hammer::QuerySub()
			{
				//���g���u�B�v�̎�
				if (GetObjectType() == EN_RO_TYPE_NAIL)
				{

				}
				//���g���u���Ɓv�̎�
				else if (GetObjectType() == EN_RO_TYPE_BAR)
				{

					//��Q�I�u�W�F�N�g�́u���v���N�G��
					QueryLOs<nsObstacleObject::OObox>(EN_OO_TYPE_BOX, [&](nsObstacleObject::OObox* box) -> bool
						{
							//���g���u���Ɓv�ŁA�u���v�ƏՓ˂������̏���
							HammerHitBox(box);

							return true;
						}
					);

				}
			}

			/**
			 * @brief ���g���u���Ɓv�ŁA�u���v�ƏՓ˂������̏���
			 * @param [in] box ���̃|�C���^
			*/
			void ROnail_hammer::HammerHitBox(nsObstacleObject::OObox* box)
			{
				//���g�Ɓu���v���Փ˂�����
				if (IsHitLevelObject(*this, *box))
				{
					//�s���ł�����
					if (m_actionFlag)
					{
						//�u���v��j��
						DeleteGO(box);
						//���g�̃I�u�W�F�N�g��j��
						DeleteGO(this);
						//�s���ł��Ȃ��悤�ɂ���
						m_actionFlag = false;

						//����Ƃ��̃T�E���h�̍Đ�
						m_brokenSE->Play(false);

						//�A�b�v�x�N�g��
						Vector3 upVec = g_VEC3_UP;
						//�A�b�v�x�N�g�������g�̉�]�ŉ�
						m_rotation.Apply(upVec);
						upVec.Scale(LENGTH_UP_POSITION_EFFECT);
						//�G�t�F�N�g�̍��W����]�𒲐߂���
						m_brokenEF->SetPosition(m_position + upVec);		//���W��n��
						m_brokenEF->SetRotation(m_rotation);
						//�G�t�F�N�g�̍Đ�
						m_brokenEF->Play();

					}
				}

				return;
			}
		}
	}
}