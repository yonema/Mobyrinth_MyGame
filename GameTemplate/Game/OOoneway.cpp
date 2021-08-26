#include "stdafx.h"
#include "OOoneway.h"

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
			//�u����ʍs�v�̒萔�f�[�^���g�p�\�ɂ���
			using namespace onewayConstData;
			//���ʒ萔�f�[�^���g�p�\�ɂ���
			using namespace nsCommonData;

			//�X�^�[�g�֐�
			bool OOoneway::StartSub()
			{
				//���f���̏������ƃ^�C�v�̐ݒ�
				Init(MODEL_FILEPATH_ONEWAY_LEFT, EN_OO_TYPE_ONEWAY);
				//���f���𖳌������Ĕ�\���ɂ���
				GetModelRender()->Deactivate();

				//���f���̏���������
				InitModel();

				//�E���������������ɍ��킹�ď���������
				InitLeftOrRight();

				//OBB�̏���������
				InitOBB();


#ifdef MY_DEBUG
				Vector3* vertPos;
				for (int i = 0; i < enLeftAndRightNum; i++)
				{
					vertPos = m_sideOBB[i].GetBoxVertex();
					for (int j = 0; j < m_vertNum; j++)
					{
						m_dbgVertMR[i][j] = NewGO<CModelRender>(0);
						m_dbgVertMR[i][j]->Init("Assets/modelData/dbgBox.tkm");
						m_dbgVertMR[i][j]->SetPosition(vertPos[j]);
					}
				}
#endif

				return true;
			}

			/**
			 * @brief ���f���̏���������
			*/
			void OOoneway::InitModel()
			{
				//�������̒ʂ�鎞�̃��f���̐����Ə�����
				m_canPassMR = NewGO<nsGraphic::nsModel::CModelRender>(PRIORITY_FIRST);
				m_canPassMR->Init(MODEL_FILEPATH_ONEWAY_LEFT_CAN_PASS, D3D12_CULL_MODE_NONE);
				//���W�A��]�A�g�嗦��ݒ�B
				m_canPassMR->SetPosition(m_position);
				m_canPassMR->SetRotation(m_rotation);
				m_canPassMR->SetScale(m_scale);
				//�A�E�g���C����`�悷��
				m_canPassMR->SetDrawOutLineFlag(true);
				//���������Ĕ�\���ɂ���
				m_canPassMR->Deactivate();

				//�E�����̃��f���̐����Ə�����
				m_rightMR = NewGO<nsGraphic::nsModel::CModelRender>(PRIORITY_FIRST);
				m_rightMR->Init(MODEL_FILEPATH_ONEWAY_RIGHT, D3D12_CULL_MODE_NONE);
				//���W�A��]�A�g�嗦��ݒ�B
				m_rightMR->SetPosition(m_position);
				m_rightMR->SetRotation(m_rotation);
				m_rightMR->SetScale(m_scale);
				//�A�E�g���C����`�悷��
				m_rightMR->SetDrawOutLineFlag(true);
				//���������Ĕ�\���ɂ���
				m_rightMR->Deactivate();

				//�E�����̒ʂ�鎞�̃��f���̐����Ə�����
				m_rightCanPassMR = NewGO<nsGraphic::nsModel::CModelRender>(PRIORITY_FIRST);
				m_rightCanPassMR->Init(MODEL_FILEPATH_ONEWAY_RIGHT_CAN_PASS, D3D12_CULL_MODE_NONE);
				//���W�A��]�A�g�嗦��ݒ�B
				m_rightCanPassMR->SetPosition(m_position);
				m_rightCanPassMR->SetRotation(m_rotation);
				m_rightCanPassMR->SetScale(m_scale);
				//�A�E�g���C����`�悷��
				m_rightCanPassMR->SetDrawOutLineFlag(true);
				//���������Ĕ�\���ɂ���
				m_rightCanPassMR->Deactivate();

				return;
			}

			/**
			 * @brief �E���������������ɍ��킹�ď���������
			*/
			void OOoneway::InitLeftOrRight()
			{
				//���������H
				if (m_leftOrRight == EN_LEFT)
				{
					//������

					//�������i�ʏ�̌����j�̒ʂ�Ȃ����f����L����
					GetModelRender()->Activate();
				}
				else
				{
					//�E����

					//�E�����i���Ό����j�̒ʂ�Ȃ����f����L����
					m_rightMR->Activate();

				}

				return;
			}

			/**
			 * @brief OBB�̏���������
			*/
			void OOoneway::InitOBB()
			{
				//OBB�̕����x�N�g���̒�����ݒ�
				SetOBBDirectionLength(SIZE_OBB_ONEWAY);

				//OBBWorld�Ɏ��g��OBB�̓o�^�����������āA����OBB���v���C���[�ƂԂ���Ȃ��悤�ɂ���
				nsOBB::COBBWorld::GetInstance()->RemoveOBB(GetOBB());

				//OBB�̏������f�[�^
				nsOBB::SInitOBBData initOBBData;
				initOBBData.width = SIZE_OBB_ONEWAY_SIDE.x;				//���FX
				initOBBData.length = SIZE_OBB_ONEWAY_SIDE.z;			//�����FZ
				initOBBData.height = SIZE_OBB_ONEWAY_SIDE.y;			//�����FY
				initOBBData.rotation = m_rotation;						//��]

				//�T�C�h��OBB�ւ̃x�N�g��
				Vector3 toSideObbVec = g_VEC3_RIGHT;
				//���g�̉�]�ŉ�
				m_rotation.Apply(toSideObbVec);
				//�T�C�h��OBB�ւ̃x�N�g����L�΂�
				toSideObbVec.Scale(LENGHT_TO_SIDE_OBB);

				//���T�C�h��OBB�̍��W
				initOBBData.position = m_position + toSideObbVec;
				//���T�C�h��OBB�̏�����
				m_sideOBB[EN_LEFT].Init(initOBBData);

				//���ւ̃x�N�g�����E�ւ̃x�N�g���ɕϊ�
				toSideObbVec.Scale(-1.0f);
				//�E�T�C�h��OBB�̍��W
				initOBBData.position = m_position + toSideObbVec;
				//�E�T�C�h��OBB�̏�����
				m_sideOBB[EN_RIGHT].Init(initOBBData);

				//���T�C�h��OBB��OBB���[���h�ɓo�^���āA�v���C���[�ƏՓ˂���悤�ɂ���
				for (int i = 0; i < EN_LEFT_AND_RIGHT_NUM; i++)
				{
					nsOBB::COBBWorld::GetInstance()->AddOBB(m_sideOBB[i]);
				}

				return;
			}


			//�f�X�g���N�^
			OOoneway::~OOoneway()
			{

				DeleteGO(m_canPassMR);
				DeleteGO(m_rightCanPassMR);
				DeleteGO(m_rightMR);
				//���T�C�h��OBB��OBB���[���h�����������
				for (int i = 0; i < EN_LEFT_AND_RIGHT_NUM; i++)
				{
					nsOBB::COBBWorld::GetInstance()->RemoveOBB(m_sideOBB[i]);
#ifdef MY_DEBUG
					for (int j = 0; j < m_vertNum; j++)
					{
						DeleteGO(m_dbgVertMR[i][j]);
					}
#endif
				}


				return;
			}

			//�A�b�v�f�[�g�֐�
			void OOoneway::UpdateSub()
			{
				//�v���C���[�����g�Ɠ��������������Ă��邩
				if (m_player->GetEnLeftOrRight() == m_leftOrRight)
				{
					//�����Ă��鎞��
					//�ʂ�邽�߁A�T�C�h��OBB���O�ݒ�ɂ���
					m_sideOBB[m_leftOrRight].SetExceptionFlag(true);

					//�ʂ�郂�f����L��������
					CanPassModelActivate();

				}
				else
				{
					//�����Ă��Ȃ�����
					//�ʂ�Ȃ����߁A�ēx��OBB���O�ݒ�ł͂Ȃ�����
					m_sideOBB[m_leftOrRight].SetExceptionFlag(false);

					//�ʂ�Ȃ����f����L��������
					CannotPassModelActivate();
				}

				//���f���̃g�����X�t�H�[���̍X�V����
				ModelTransformUpdate();

				return;
			}

			/**
			 * @brief �ʂ�郂�f���̗L��������
			*/
			void OOoneway::CanPassModelActivate()
			{
				//�E�����ǂ��炩�̒ʂ�郂�f����L�������A�����Е��͖���������

				//���������H
				if (m_leftOrRight == EN_LEFT)
				{
					//������

					//�������̒ʂ�郂�f����L����
					m_canPassMR->Activate();
					//�E�����̒ʂ�郂�f���𖳌���
					m_rightCanPassMR->Deactivate();
				}
				else
				{
					//�E����

					//�������̒ʂ�郂�f���𖳌���
					m_canPassMR->Deactivate();
					//�E�����̒ʂ�郂�f����L����
					m_rightCanPassMR->Activate();
				}

				//�ʂ�Ȃ����f���͗������������Ĕ�\���ɂ���
				GetModelRender()->Deactivate();
				m_rightMR->Deactivate();

				return;
			}

			/**
			 * @brief �ʂ�Ȃ����f����L��������
			*/
			void OOoneway::CannotPassModelActivate()
			{
				//�E�����ǂ��炩�̒ʂ�Ȃ����f����L�������A�����Е��͖���������

				//���������H
				if (m_leftOrRight == EN_LEFT)
				{
					//������

					//�������̒ʂ�Ȃ����f����L����
					GetModelRender()->Activate();
					//�E�����̒ʂ�Ȃ����f���𖳌���
					m_rightMR->Deactivate();
				}
				else
				{
					//�E����

					//�������̒ʂ�Ȃ����f���𖳌���
					GetModelRender()->Deactivate();
					//�E�����̒ʂ�Ȃ����f����L����
					m_rightMR->Activate();
				}

				//�ʂ�郂�f���͗������������āA��\���ɂ���
				m_canPassMR->Deactivate();
				m_rightCanPassMR->Deactivate();

				return;
			}

			/**
			 * @brief ���f���̃g�����X�t�H�[���̍X�V
			*/
			void OOoneway::ModelTransformUpdate()
			{
				//�L���Ȃ�X�V����
				if (m_canPassMR->IsActive())
				{
					//���W�A��]�A�g�嗦��ݒ肷��
					m_canPassMR->SetPosition(m_position);
					m_canPassMR->SetRotation(m_rotation);
					m_canPassMR->SetScale(m_scale);
				}
				//�L���Ȃ�X�V����
				if (m_rightCanPassMR->IsActive())
				{
					//���W�A��]�A�g�嗦��ݒ肷��
					m_rightCanPassMR->SetPosition(m_position);
					m_rightCanPassMR->SetRotation(m_rotation);
					m_rightCanPassMR->SetScale(m_scale);
				}
				//�L���Ȃ�X�V����
				if (m_rightMR->IsActive())
				{
					//���W�A��]�A�g�嗦��ݒ肷��
					m_rightMR->SetPosition(m_position);
					m_rightMR->SetRotation(m_rotation);
					m_rightMR->SetScale(m_scale);
				}

				return;
			}

		}
	}
}