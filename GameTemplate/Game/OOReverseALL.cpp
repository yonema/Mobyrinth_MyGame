#include "stdafx.h"
#include "OOReverseALL.h"

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
			//�u�S���]�v�̒萔�f�[�^���g�p�\�ɂ���
			using namespace reverseAllConstData;

			//�X�^�[�g�֐�
			bool OOReverseALL::StartSub()
			{
				//���f���̏������ƃ^�C�v�̐ݒ�
				Init(MODEL_FILEPATH_REVERSE_ALL, EN_OO_TYPE_REVERSE_ALL);

				//OBB�̃p�����[�^�[��ݒ肷��
				SetOBBDirectionLength(SIZE_OBB_REVERSE_ALL);

				//���f�����\���ɂ���
				GetModelRender()->Deactivate();

				//OBBWorld�Ɏ��g��OBB�̓o�^���������āA�v���C���[�ƏՓ˂��Ȃ��悤�ɂ���
				nsOBB::COBBWorld::GetInstance()->RemoveOBB(GetOBB());

				//�I�u�W�F�N�g�Ɠ�����Ȃ��悤�ɂ���
				SetIsHitFlag(false);

				//���Ȕ����F��ݒ�
				SetModelEmissionColor(MODEL_COLOR_EMISSION);

				//�T�E���h�̏���������
				InitSound();

				//�G�t�F�N�g�̏���������
				InitEffect();


				//�f�o�b�N�p
#ifdef MY_DEBUG
	//��ŏ���
				Vector3* vertPos = GetOBB().GetBoxVertex();
				for (int i = 0; i < m_vertNum; i++)
				{
					//OBB�̒��_�����邽�߂̃��f���̐����Ə�����
					m_dbgVertPosMR[i] = NewGO<CModelRender>(0);
					m_dbgVertPosMR[i]->Init("Assets/modelData/dbgBox.tkm");
					m_dbgVertPosMR[i]->SetPosition(vertPos[i]);
					m_dbgVertPosMR[i]->SetRotation(m_rotation);
				}
				//�f�o�b�N�p�����܂�
#endif
				return true;
			}

			/**
			 * @brief �T�E���h�̏���������
			*/
			void OOReverseALL::InitSound()
			{
				//changeSE�̃T�E���h�L���[�𐶐�����
				m_changeAllSC = NewGO<nsSound::CSoundCue>(nsCommonData::PRIORITY_FIRST);
				//changeSE�̃T�E���h�L���[���Awave�t�@�C�����w�肵�ď���������B
				m_changeAllSC->Init(SOUND_FILEPATH_CHANGE_ALL);
				//���ʒ���
				m_changeAllSC->SetVolume(SOUND_VALUME_CHANGE_ALL);

				return;
			}

			/**
			 * @brief �G�t�F�N�g�̏���������
			*/
			void OOReverseALL::InitEffect()
			{
				//�S���]���g��\���G�t�F�N�g�̐����Ə�����
				m_reverslAllEF = NewGO<Effect>(nsCommonData::PRIORITY_FIRST);
				m_reverslAllEF->Init(EFFECT_FILEPATH_REVERSE_ALL);
				//���W�Ɖ�]�Ɗg�嗦��ݒ肷
				m_reverslAllEF->SetPosition(m_position);
				m_reverslAllEF->SetRotation(m_rotation);
				m_reverslAllEF->SetScale(EFFECT_SCALE_REVERSE_ALL);
				//�Đ�����
				m_reverslAllEF->Play();

				//�S���]���鎞�̃G�t�F�N�g�̐����Ə�����
				m_changeAllEF = NewGO<Effect>(nsCommonData::PRIORITY_FIRST);
				m_changeAllEF->Init(EFFECT_FILEPATH_CHANGE_ALL);
				//���W�Ɖ�]�Ɗg�嗦��ݒ肷
				m_changeAllEF->SetPosition(m_position);
				m_changeAllEF->SetRotation(m_rotation);
				m_changeAllEF->SetScale(EFFECT_SCALE_CHANGE_ALL);

				return;
			}

			//�f�X�g���N�^
			OOReverseALL::~OOReverseALL()
			{
				//�G�t�F�N�g���Đ����Ȃ�
				if (m_reverslAllEF->IsPlay())
					//��~����
					m_reverslAllEF->Stop();

				//�S���]���g��\���G�t�F�N�g��j��
				DeleteGO(m_reverslAllEF);

				//�S���]���鎞�̃G�t�F�N�g��j��
				DeleteGO(m_changeAllEF);

				//�T�E���h�̔j��
				DeleteGO(m_changeAllSC);

#ifdef MY_DEBUG
				//�f�o�b�N�p
				//��ŏ���
				for (int i = 0; i < m_vertNum; i++)
				{
					//OBB�̒��_�����邽�߂̃��f���̔j��
					DeleteGO(m_dbgVertPosMR[i]);
				}
				//�f�o�b�N�p�����܂�
#endif

				return;
			}

			//�|�[�Y���ł����ł��A�b�v�f�[�g����֐�
			void OOReverseALL::AlwaysUpdate()
			{
				//�G�t�F�N�g�Đ��܂ł̃^�C�}�[
				m_effectLoopTimer += nsTimer::GameTime().GetFrameDeltaTime();

				//���[�v�Đ��ɂ���
				if (m_effectLoopTimer >= EFFECT_TIME_LOOP) {
					m_reverslAllEF->Play();
					m_effectLoopTimer = 0;
				}

				return;
			}


			//�A�b�v�f�[�g�֐�
			void OOReverseALL::UpdateSub()
			{

				//�A�b�v�f�[�g�X�e�[�g�ŏ�����U�蕪����
				//switch (m_updateState)
				//{
				//case enBeforHitPlayer:
				//	//�v���C���[�ƏՓˑO�̏���
				//	BeforeHitPlayer();
				//	break;
				//case enHitPlayer:
				//	//�v���C���[�ƏՓˎ��̏���
				//	HitPlayer();
				//	break;
				//case enAfterHitPlayer:
				//	//�v���C���[�ƏՓˌ�̏���
				//	AfterHitPlayer();
				//	break;
				//}


				///
				///
				/// switch��else if�ɂ��Ă��܂��ƁA
				/// �ړI�̏�����1�t���[���x��Ă��܂�����
				/// �S��if�ŐU�蕪�����B

				//�A�b�v�f�[�g�X�e�[�g�ŏ�����U�蕪����
				if (m_updateState == EN_BEFOR_HIT_PLAYER)
					//�v���C���[�ƏՓˑO�̏���
					BeforeHitPlayer();

				if (m_updateState == EN_HIT_PLAYER)
					//�v���C���[�ƏՓˎ��̏���
					HitPlayer();

				if (m_updateState == EN_AFTER_HIT_PLAYER)
					//�v���C���[�ƏՓˌ�̏���
					AfterHitPlayer();

				return;
			}

			//�v���C���[�ƏՓˑO�̏���
			void OOReverseALL::BeforeHitPlayer()
			{
				//�v���C���[�Ƃ̏Փ˂𒲂ׂ�
				if (IsHitPlayer())
				{
					//�Փ˂��Ă���

					//�Փˎ��̃v���C���[�̍��W��ێ�
					m_playerHitPosition = m_player->GetPosition();
					//�A�b�v�f�[�g�X�e�[�g���v���C���[�ƏՓˎ��̏�Ԃ�
					m_updateState = EN_HIT_PLAYER;

				}
				else
				{
					//�Փ˂��Ă��Ȃ�

					//�ՓˑO�̃v���C���[�̍��W��ێ�
					m_playerBeforePosition = m_player->GetPosition();
				}

				return;
			}

			//�v���C���[�ƏՓˎ��̏���
			void OOReverseALL::HitPlayer()
			{
				//�v���C���[�Ƃ̏Փ˂𒲂ׂ�
				if (IsHitPlayer())
				{
					//�Փ˂��Ă���

					//�Փˎ��̃v���C���[�̍��W��ێ�
					m_playerHitPosition = m_player->GetPosition();
				}
				else
				{
					//�Փ˂��Ă��Ȃ�

					//�Փˌ�̃v���C���[�̍��W�̊m��
					m_playerAfterPosition = m_player->GetPosition();
					//�A�b�v�f�[�g�X�e�[�g���v���C���[�ƏՓˌ�̏�Ԃ�
					m_updateState = EN_AFTER_HIT_PLAYER;
				}

				return;
			}

			//�v���C���[�ƏՓˌ�̏���
			void OOReverseALL::AfterHitPlayer()
			{
				//�Փˌ�̃v���C���[�̍��W�̊m��
				m_playerAfterPosition = m_player->GetPosition();

				//�ՓˑO����Փˎ��̍��W�ւ̃x�N�g��
				Vector3 beforeToHit = m_playerHitPosition - m_playerBeforePosition;
				//�Փˎ�����Փˌ�̍��W�ւ̃x�N�g��
				Vector3 hitToAfter = m_playerAfterPosition - m_playerHitPosition;
				//���K�����Ă���
				beforeToHit.Normalize();
				hitToAfter.Normalize();
				//��̓��ς����
				float inner = Dot(beforeToHit, hitToAfter);

				//���ςœ�̃x�N�g���̌����𒲂ׂ�
				if (inner >= 0.0f)
				{
					//���A���������Ȃ�

					//���x���I�u�W�F�N�g������Ă���
					std::vector<ILevelObjectBase*> levelObjects
						= CLevelObjectManager::GetInstance()->GetLevelObjects();
					//�S�Ẵ��x���I�u�W�F�N�g�Ɍ���
					for (ILevelObjectBase* levelObject : CLevelObjectManager::GetInstance()->GetLevelObjects())
					{
						//�����I�u�W�F�N�g�ł͖���������
						if (levelObject->GetIsHitFlag())
						{
							//���]������
							levelObject->AllReverse();
						}
					}
					//changeSE�����[�v�Đ����I�t�ōĐ�����B
					m_changeAllSC->Play(false);

					//��ɂ�����x�N�g��
					Vector3 upVec = g_VEC3_UP;
					m_rotation.Apply(upVec);
					upVec.Scale(EFFECT_LENGHT_POSITION_CHANGE_ALL);
					m_changeAllEF->SetPosition(m_position + upVec);		//���W��n��
					m_changeAllEF->SetRotation(m_rotation);
					m_changeAllEF->Play();								//�Đ�
				}


				//�ՓˑO�̃v���C���[�̍��W��ێ�
				m_playerBeforePosition = m_player->GetPosition();

				//�A�b�v�f�[�g�X�e�[�g���v���C���[�ƏՓˑO�̏�Ԃ�
				m_updateState = EN_BEFOR_HIT_PLAYER;

				return;
			}

		}
	}
}