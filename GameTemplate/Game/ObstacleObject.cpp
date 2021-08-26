#include "stdafx.h"
#include "ObstacleObject.h"

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

			//�X�^�[�g�֐�
			bool CObstacleObject::PureVirtualStart()
			{
				//OBBWorld�Ɏ��g��OBB��o�^����
				//����ɂ����Player���ʂ�Ȃ��Ȃ�
				nsOBB::COBBWorld::GetInstance()->AddOBB(GetOBB());

				//�\���ɂ��邩�����ɂ��邩���ׂ�
				//���]�I�u�W�F�N�g�ł͂Ȃ�����false��n��
				CheckFrontOrBackSide(false);

				//�I�[�o�[���C�h���Ăق����֐�StartSub()�͂����ŌĂ΂��B
				return StartSub();
			}


			//�f�X�g���N�^
			CObstacleObject::~CObstacleObject()
			{
				//LevelObjectManager�ɂ��̃I�u�W�F�N�g�͏�����I���ē`����B
				Delete();

				//OBBWorld�Ɏ��g��OBB�̓o�^������������
				nsOBB::COBBWorld::GetInstance()->RemoveOBB(GetOBB());

				//���f���̏���
				DeleteGO(m_modelRender);

				return;
			}



			/// <summary>
			/// �������֐�
			/// �ŏ��ɌĂ�łˁBtrue��߂��ĂˁB
			/// ���f���̃t�@�C���p�X�ƃI�u�W�F�N�g�̃^�C�v��
			/// �ݒ肷��
			/// �^�C�v�ꗗ��ObjectType.h���Q��
			/// </summary>
			/// <param name="filePath">���f����tkm�t�@�C���p�X</param>
			/// <param name="objectType">�^�C�v</param>
			/// <returns>true��߂��Ă�</returns>
			bool CObstacleObject::Init(const char* filePath, int objectType)
			{
				//���f�������_���[�𐶐�
				m_modelRender = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::PRIORITY_FIRST);

				//�����I�u�W�F�N�g���H
				if (GetFlagTransparentObject())
				{
					//�����I�u�W�F�N�g�̏ꍇ

					//���f���̏������f�[�^
					ModelInitData modelInitData;
					//�V�F�[�_�[��ݒ�
					//�����I�u�W�F�N�g���`��ł���V�F�[�_�[��ݒ�
					modelInitData.m_fxFilePath = "Assets/shader/model2.fx";

					//�V���h�E�}�b�v�̓o�^
					modelInitData.m_expandShaderResoruceView[0] = &g_graphicsEngine->GetShadowMap().GetShadowBlur();
					//ZPrepass�ō쐬���ꂽ�[�x�e�N�X�`���̓o�^
					modelInitData.m_expandShaderResoruceView[1] = &g_graphicsEngine->GetZPrepassDepthTexture();
					//�X�J�C�L���[�u�̓o�^
					modelInitData.m_expandShaderResoruceView[2] = &g_graphicsEngine->GetSkyCubeTexture();

					//�J�����O���[�h��ݒ�
					modelInitData.m_cullMode = D3D12_CULL_MODE_NONE;

					//���f�������_���[��������
					//���f���̃t�@�C���p�X�̐ݒ�
					modelInitData.m_tkmFilePath = filePath;
					//���f���̏�����
					m_modelRender->Init(modelInitData);

					//�f�B�t�@�\�h�����_�����O���s��Ȃ��悤�ɂ���
					m_modelRender->SetIsDefferdRender(false);

				}
				else
				{
					//�����A�C�e���ł͂Ȃ���

					//���ʂɏ�����

					//���f�������_���[��������
					m_modelRender->Init(filePath, D3D12_CULL_MODE_NONE);
				}


				m_modelRender->SetShadowReceiverFlag(false);
				//�I�u�W�F�N�g�̃^�C�v��ݒ肷��
				SetObjectType(objectType);
				m_modelRender->SetDrawOutLineFlag(true);

				return true;
			}

			//�A�b�v�f�[�g�֐�
			void CObstacleObject::PureVirtualUpdate()
			{
				//�v���C���[���������Ă��Ȃ�������
				if (!m_player)
				{
					//�v���C���[��T��
					m_player = CLevelObjectManager::GetInstance()->GetPlayer();

					//����ł�������Ȃ������牽������return
					if (!m_player)
						return;
				}

				//���f�������_���[�̏ꏊ�Ɖ�]�Ɗg��
				m_modelRender->SetPosition(m_position);
				m_modelRender->SetRotation(m_rotation);
				m_modelRender->SetScale(m_scale);

				//�I�[�o�[���C�h���Ăق����֐�UpdateSub()
				UpdateSub();

				return;
			}

		}
	}
}