#include "stdafx.h"
#include "OOwall.h"
#include "ROrunning_stop.h"

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
			//ObstacleObject�̃��f���̃t�@�C���p�X��OBB�̃T�C�Y�̒萔�f�[�^���g�p�\�ɂ���
			using namespace OOsFilepathAndObbSizeConstData;
			//�u�ǁv�̒萔�f�[�^���g�p�\�ɂ���
			using namespace wallConstData;

			//�R���X�g���N�^
			OOwall::OOwall()
			{
				//�X�^�[�g�֐��ł͊Ԃɍ���Ȃ��B
				//�I�u�W�F�N�g���������ꂽ�u�ԂɌĂ΂�Ăق�������

				//�E�F�C�|�C���g����̏�̋�����ݒ�
				SetYPosLen(LENGHT_POSITION_FROM_STAGE);

				return;
			}

			//�X�^�[�g�֐�
			bool OOwall::StartSub()
			{
				//�������p�֐�
				Init(MODEL_FILEPATH_WALL, EN_OO_TYPE_WALL);

				//OBB�̃T�C�Y��ݒ�
				SetOBBDirectionLength(MODEL_SCALE_WALL);

				//OBB�̃^�O��ݒ肷��
				SetOBBTag(nsOBB::COBB::EN_OO_TYPE_WALL);


				//�ړ��O�̏����ʒu�ƈړ���̏I�[�ʒu�̏���������
				InitStartAndEndPos();

				//�ғ��A��~�I�u�W�F�N�g�̏���������
				InitRun_stopObject();

				//�T�E���h�̏���������
				InitSound();

				return true;
			}

			/**
			 * @brief �ړ��O�̏����ʒu�ƈړ���̏I�[�ʒu�̏���������
			*/
			void OOwall::InitStartAndEndPos()
			{
				//�ړ��O�̏����ʒu�̐ݒ�
				m_startPosition = m_position;

				//�A�b�v�x�N�g��
				Vector3 upVec = g_VEC3_UP;
				//���݂̎��g�̉�]�ŁA�A�b�v�x�N�g������
				m_rotation.Apply(upVec);
				//�A�b�v�x�N�g��
				upVec.Scale(LENGTH_MOVE * TIME_MOVE);
				//�ړ���̏I�[�ʒu�̐ݒ�
				m_endPosition = m_startPosition + upVec;

				return;
			}

			/**
			 * @brief �ғ��A��~�I�u�W�F�N�g�̏���������
			*/
			void OOwall::InitRun_stopObject()
			{
				//�ŏ��ɓ���Ă������ғ��A��~�I�u�W�F�N�g��
				//��~�Ő�������
				m_pRun_stop = NewGO<nsReversibleObject::ROrunning_stop>(nsCommonData::PRIORITY_FIRST);
				m_pRun_stop->SetPosition(m_position);
				m_pRun_stop->SetFrontOrBack(nsCommonData::EN_BACK);
				//�S���]���Ȃ��悤�ɂƁATips��\�����Ȃ��悤�ɂ���
				m_pRun_stop->SetLock(true);

				return;
			}

			/**
			 * @brief �T�E���h�̏���������
			*/
			void OOwall::InitSound()
			{
				//�ǂ������Ƃ��̃T�E���h�𐶐�����
				m_wallmoveSE = NewGO<nsSound::CSoundCue>(nsCommonData::PRIORITY_FIRST);
				//�ǂ������Ƃ��̃T�E���h���Awave�t�@�C�����w�肵�ď���������B
				m_wallmoveSE->Init(SOUND_FILEPATH_WALL_MOVE);
				//���ʒ���
				m_wallmoveSE->SetVolume(SOUND_VALUME_WALL_MOVE);

				return;
			}

			//�f�X�g���N�g
			OOwall::~OOwall()
			{
				DeleteGO(m_wallmoveSE);

				//�ғ��A��~�I�u�W�F�N�g��ILevelObjectBase���p�����Ă��邩��A�����ŏ����K�v�͂Ȃ�

				return;
			}

			//�A�b�v�f�[�g�֐�
			void OOwall::UpdateSub()
			{
				//�ŏ��̃A�b�v�f�[�g���H
				if (m_firstUpdateFlag)
					FirstUpdate();

				//�ғ������H
				if (m_moveFlag)
				{
					//���W�̍X�V����
					PositionUpdate();

					//�T�E���h�̍X�V����
					SEUpdate();
				}

				return;
			}

			/**
			 * @brief ���W�̍X�V����
			*/
			void OOwall::PositionUpdate()
			{
				//�����ʒu����I�[�ʒl�ւ̃x�N�g��
				Vector3 movePos = m_endPosition - m_startPosition;	//�I�[�ʒu�ւ̃x�N�g��

				//�ړ����x�̔{��
				float moveScale = 0.0f;

				//�^�C�}�[���Г����̎��Ԃ�菬�������H
				if (m_moveTimer < TIME_MOVE)
				{
					//�^�C�}�[���Г����̎��Ԃ�菬�����Ƃ�

					//���݂̎��� / �Г����̎��� �Ŕ{�����o��
					moveScale = m_moveTimer / TIME_MOVE;
				}
				//�^�C�}�[���������̎��Ԃ�菬�������H
				else if (m_moveTimer < TIME_MOVE * 2)
				{
					//�^�C�}�[���Г����̎��Ԃ��傫���A���A
					//�������̎��Ԃ�菬�����Ƃ�

					//�I�[�ʒu�ւ̎c��̃^�C�}�[���o���āA�{�����o��
					moveScale = (TIME_MOVE * 2.0f - m_moveTimer) / TIME_MOVE;
				}
				else
				{
					//�^�C�}�[���������̎��Ԃ��傫����

					//�^�C�}�[������������
					m_moveTimer = 0.0f;
				}

				//�ړ���ւ̃x�N�g���ɁA�{�����|����
				movePos.Scale(moveScale);

				//���݂̏ꏊ���A�����ʒu����ړ���ւ̃x�N�g�������Z�����ꏊ�ɂ���
				m_position = m_startPosition + movePos;

				//�J�E���^�[��i�߂�
				m_moveTimer += nsTimer::GameTime().GetFrameDeltaTime();

				return;
			}

			/**
			 * @brief �T�E���h�̍X�V����
			*/
			void OOwall::SEUpdate()
			{
				//�v���C���[���玩�g�ւ̃x�N�g��
				Vector3 fromPlayerVec = m_position - m_player->GetPosition();
				//�v���C���[����̋���
				const float distFromPlayer = fromPlayerVec.Length();

				//�������ő勗����菬�������H
				if (distFromPlayer < DISTANCE_MAX_WALL_MOVE_SOUND)
				{
					//���ʂ̔{��
					//distFromPlayer���������Ȃ�قǁA�{�����`1.0f�ɋ߂Â�
					const float volumeRate =
						(DISTANCE_MAX_WALL_MOVE_SOUND - distFromPlayer) / DISTANCE_MAX_WALL_MOVE_SOUND;
					//���ʌv�Z
					const float volume = SOUND_VALUME_WALL_MOVE * volumeRate;


					//UFOmoveSE�����[�v�Đ����I���ōĐ�����B
					m_wallmoveSE->Play(true);

					//���ʒ���
					m_wallmoveSE->SetVolume(volume);

				}
				else
				{
					//�������ő勗�����傫��

					//�Đ������H
					if (m_wallmoveSE->IsPlaying())
					{
						//�Đ����Ȃ��~����
						m_wallmoveSE->Stop();
					}
				}

				return;
			}

			/// <summary>
			/// ���ڂ̃A�b�v�f�[�g�ł����Ă΂��֐�
			/// </summary>
			void OOwall::FirstUpdate()
			{
				//�ғ��A��~�I�u�W�F�N�g���N�G������ƁA
				//m_pRun_stop�̒��g�̃I�u�W�F�N�g�������Ă��܂�����A
				//�������񃍁[�J���ϐ��̃|�C���^�ɔ����Ă�����
				//m_pRun_stop�ɂ�nullptr�����Ă���
				nsReversibleObject::ROrunning_stop* pRun_stop = m_pRun_stop;
				m_pRun_stop = nullptr;
				//�N�G�������ĕǂ�T��
				pRun_stop->QuerySub();

				//���ڂ̃A�b�v�f�[�g�̏I��
				m_firstUpdateFlag = false;

				return;
			}
		}
	}
}