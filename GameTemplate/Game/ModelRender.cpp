#include "stdafx.h"
#include "ModelRender.h"


/**
 * @brief ���r�����X
*/
namespace nsMobyrinth
{
	/**
	 * @brief �O���t�B�b�N
	*/
	namespace nsGraphic
	{
		/**
		 * @brief ���f��
		*/
		namespace nsModel
		{

			//���f�������_���[�̒萔�f�[�^���g����悤�ɂ���
			using namespace modelRenderConstData;

			/// <summary>
			/// �f�t�H���g�̏����������֐�
			/// �ŏ��ɌĂ�ŁI
			/// </summary>
			/// <param name="filePath">tkm�̃t�@�C���p�X</param>
			/// <param name="animationClips">�A�j���[�V�����N���b�v</param>
			/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̑���</param>
			/// <param name="modelUpAxis">���f����UP��</param>
			void CModelRender::Init(
				const char* filePath,
				D3D12_CULL_MODE cullMode,
				AnimationClip* animationClips,
				int numAnimationClips,
				EnModelUpAxis modelUpAxis)
			{
				//tkm�̃t�@�C���p�X��ێ�
				m_tkmFilePath = filePath;
				//�X�P���g���̃f�[�^�̓ǂݍ��݁B
				InitSkeleton(filePath);
				//���f����������
				InitModel(filePath, cullMode, modelUpAxis);
				//�A�j���[�V������������
				InitAnimation(animationClips, numAnimationClips);
				SetShadowCasterFlag(true);
				SetShadowReceiverFlag(true);

				InitZPrepassModel();
				//����������
				m_isInited = true;

				return;
			}


			/// <summary>
			/// �J�X�^���p�̏������֐�
			/// �ŏ��ɌĂ�ŁI
			/// �f�t�H���g��ModelInitData�Ƃ͈Ⴄ���e�ŏ���������Ƃ��Ɏg��
			/// </summary>
			/// <param name="initData">�������p�̃f�[�^</param>
			/// <param name="animationClips">�A�j���[�V�����N���b�v</param>
			/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̑���</param>
			void CModelRender::Init(
				ModelInitData initData,
				AnimationClip* animationClips,
				int numAnimationClips)
			{
				//tkm�̃t�@�C���p�X��ێ�
				m_tkmFilePath = initData.m_tkmFilePath;
				//�X�P���g���̃f�[�^�̓ǂݍ��݁B
				InitSkeleton(initData.m_tkmFilePath);
				//���f����������
				m_model.Init(initData);
				//�A�j���[�V������������
				InitAnimation(animationClips, numAnimationClips);

				InitZPrepassModel();


				//����������
				m_isInited = true;

				return;
			}


			/// <summary>
			/// �X�P���g�����������B
			/// tkm�t�@�C���p�X��tks�t�@�C���p�X�ɕϊ�����
			/// </summary>
			/// <param name="filePath">tkm�t�@�C���p�X</param>
			/// <returns>�������������������H</returns>
			bool CModelRender::InitSkeleton(const char* filePath)
			{
				//tkm�t�@�C����tks�t�@�C���ɕϊ�����
				std::string skeletonFilePath = filePath;
				//�t�@�C���p�X�̕����񂩂�A�g���q�̏ꏊ��T��
				int pos = (int)skeletonFilePath.find(FILE_EXTENSION_TKM);
				//tkm�̃t�@�C���p�X����tks�̃t�@�C���p�X�ɓ���ւ���
				skeletonFilePath.replace(pos, FILE_EXTENSION_LENGHT, FILE_EXTENSION_TKS);

				//�X�P���g���̃��\�[�X�̊m��
				m_skeletonPtr.reset(new Skeleton);
				//�X�P���g���̃f�[�^��ǂݍ��݁B
				bool isInited = m_skeletonPtr->Init(skeletonFilePath.c_str());

				//�������ɐ����������H
				if (isInited)
					return true;	//����������true��Ԃ�
				else
				{
					//���s������X�P���g���̃��\�[�X���J������
					//false��Ԃ�
					m_skeletonPtr.reset();
					return false;
				}
			}


			/// <summary>
			/// ���f�����������B
			/// </summary>
			/// <param name="filePath">tkm�t�@�C���p�X</param>
			/// <param name="modelUpAxis">���f����UP��</param>
			void CModelRender::InitModel(const char* filePath, D3D12_CULL_MODE cullMode, EnModelUpAxis modelUpAxis)
			{
				//���̊֐������
				//���C�g���g�p�\�ɂ���
				using namespace nsLight;

				//�������f�[�^���쐬����
				ModelInitData initData;
				//tkm�t�@�C���̃t�@�C���p�X���w�肷��B
				initData.m_tkmFilePath = filePath;
				//�V�F�[�_�[�t�@�C���̃t�@�C���p�X���w�肷��B
				initData.m_fxFilePath = SHADER_FILEPATH_DEFAULT;
				//�X�P���g�����w�肷��B
				if (m_skeletonPtr)	//�X�P���g��������������Ă�����
					initData.m_skeleton = m_skeletonPtr.get();
				//���f���̏�������w�肷��B
				initData.m_modelUpAxis = modelUpAxis;

				//�V�F�[�_�ɓn���R���X�^���g�o�b�t�@�̐ݒ�

				//���C�g���ʂ̃f�[�^�̓o�^
				initData.m_expandConstantBuffer[0] =
					CLightManager::GetInstance()->GetLightParam();
				initData.m_expandConstantBufferSize[0] =
					sizeof(*CLightManager::GetInstance()->GetLightParam());

				//�f�B���N�V�������C�g�B�̃f�[�^�̓o�^
				initData.m_expandConstantBuffer[1] =
					CLightManager::GetInstance()->GetDirectionLigData();
				initData.m_expandConstantBufferSize[1] =
					sizeof(*CLightManager::GetInstance()->GetDirectionLigData()) *
					CLightManager::GetMax_DirectionLight();

				//�|�C���g���C�g�B�̃f�[�^�̓o�^
				initData.m_expandConstantBuffer[2] =
					CLightManager::GetInstance()->GetPointLigData();
				initData.m_expandConstantBufferSize[2] =
					sizeof(*CLightManager::GetInstance()->GetPointLigData()) *
					CLightManager::GetMax_PointLight();

				//�X�|�b�g���C�g�B�̃f�[�^�̓o�^
				initData.m_expandConstantBuffer[3] =
					CLightManager::GetInstance()->GetSpotLigData();
				initData.m_expandConstantBufferSize[3] =
					sizeof(*CLightManager::GetInstance()->GetPointLigData()) *
					CLightManager::GetMax_SpotLight();

				//�V���h�E�̃f�[�^�̓o�^
				initData.m_expandConstantBuffer[4] =
					g_graphicsEngine->GetShadowMap().GetShadowParam();
				initData.m_expandConstantBufferSize[4] =
					sizeof(*g_graphicsEngine->GetShadowMap().GetShadowParam()) * g_MAX_SHADOW_NUM;

				//�V�F�[�_�[���\�[�X�r���[�̐ݒ�

				//�V���h�E�}�b�v�̓o�^
				initData.m_expandShaderResoruceView[0] = &g_graphicsEngine->GetShadowMap().GetShadowBlur();
				//ZPrepass�ō쐬���ꂽ�[�x�e�N�X�`���̓o�^
				initData.m_expandShaderResoruceView[1] = &g_graphicsEngine->GetZPrepassDepthTexture();
				//�X�J�C�L���[�u�̓o�^
				initData.m_expandShaderResoruceView[2] = &g_graphicsEngine->GetSkyCubeTexture();

				//�J�����O���[�h��ݒ肷��
				initData.m_cullMode = cullMode;

				//�쐬�����������f�[�^�����ƂɃ��f��������������A
				m_model.Init(initData);

				return;
			}


			/// <summary>
			/// �A�j���[�V�������������B
			/// </summary>
			/// <param name="animationClips">�A�j���[�V�����N���b�v</param>
			/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̑���</param>
			void CModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips)
			{
				//�A�j���[�V�����N���b�v���o�^����Ă��邩
				if (animationClips == nullptr)
					return;	//����Ă��Ȃ��ꍇ�͉������Ȃ�

				//�A�j���[�V�����̃��\�[�X�̊m��
				m_animationPtr.reset(new Animation);
				//�A�j���[�V�������������B
				m_animationPtr->Init(
					*m_skeletonPtr,			//�A�j���[�V�����𗬂����ރX�P���g���B
					animationClips,		//�A�j���[�V�����N���b�v�B
					numAnimationClips	//�A�j���[�V�����̐��B
				);

				return;
			}

			/// <summary>
			/// �V���h�E�p�̃��f����������
			/// </summary>
			void CModelRender::InitShadowModel()
			{
				//�V���h�E�p�̃��f���N���X�̏�����
				m_shadowModel.Init();

				//�V���h�E�p�̃��f���̏������f�[�^
				ModelInitData initShadowModelData;
				initShadowModelData.m_fxFilePath = SHADER_FILTPATH_SHADOW_MAP;
				//�V���h�E�̃p���[���[�^��萔�o�b�t�@�ɓn��
				initShadowModelData.m_expandConstantBuffer[0] =
					(void*)g_graphicsEngine->GetShadowMap().GetShadowParam();
				initShadowModelData.m_expandConstantBufferSize[0] =
					sizeof(*g_graphicsEngine->GetShadowMap().GetShadowParam());

				initShadowModelData.m_colorBufferFormat = DXGI_FORMAT_R32G32_FLOAT;

				initShadowModelData.m_tkmFilePath = m_tkmFilePath;

				//�V���h�E�p�̃��f���̏�����
				m_shadowModel.InitModel(initShadowModelData);
				//�p�����[�^���X�V����
				m_shadowModel.UpdateModel(
					m_position,
					m_rotation,
					m_scale
				);

				return;
			}

			/// <summary>
			/// Z�v���p�X�p�̃��f����������
			/// </summary>
			void CModelRender::InitZPrepassModel()
			{
				//Z�v���p�X�p���f���̏������f�[�^
				ModelInitData modelInitData;
				modelInitData.m_tkmFilePath = m_tkmFilePath;
				modelInitData.m_fxFilePath = SHADER_GILEPATH_Z_PREPASS;
				modelInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
				modelInitData.m_trans = false;
				modelInitData.m_cullMode = D3D12_CULL_MODE_NONE;

				//Z�v���p�X�p�̃��f���̏�����
				m_zprepassModel.Init(modelInitData);

				return;
			}


			//�A�b�v�f�[�g�֐�
			void CModelRender::Update()
			{
				//����������Ă��邩�H
				if (!m_isInited)
					return;	//����Ă��Ȃ��ꍇ�͉������Ȃ�


				//�X�P���g�����X�V�B
				if (m_skeletonPtr)	//�X�P���g��������������Ă�����
					m_skeletonPtr->Update(m_model.GetWorldMatrix());
				//�A�j���[�V������i�߂�B
				if (m_animationPtr)	//�A�j���[�V����������������Ă�����
					m_animationPtr->Progress(nsTimer::GameTime().GetFrameDeltaTime());

				//���f���̍��W�X�V
				m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);

				//�V���h�E�p�̃��f��������������Ă�����
				if (m_shadowModel.IsValid())
					//�p�����[�^�X�V
					m_shadowModel.UpdateModel(m_position, m_rotation, m_scale);

				//ZPrepass�p���f���̍X�V
				m_zprepassModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);

				return;
			}

			//��ɌĂ΂��A�b�v�f�[�g�֐�
			void CModelRender::AlwaysUpdate()
			{
				//�֊s����`�悷�邩�H
				if (m_drawOutLineFlag)
				{
					// ZPrepass�̕`��p�X�Ƀ��f����ǉ�
					g_graphicsEngine->Add3DModelToZPrepass(m_zprepassModel);
				}

				return;
			}


			//�`��p�֐�
			void CModelRender::Render(RenderContext& rc)
			{
				//����������Ă��邩�H
				if (!m_isInited)
					return;	//����Ă��Ȃ��ꍇ�͉������Ȃ�

				if (!IsDefferdRender())
					return;

				//���f����`��
				m_model.Draw(rc);

				return;
			}

			void CModelRender::ForwardRender(RenderContext& rc)
			{
				if (IsDefferdRender())
					return;

				//���f����`��
				m_model.Draw(rc);

				return;
			}

		}
	}
}