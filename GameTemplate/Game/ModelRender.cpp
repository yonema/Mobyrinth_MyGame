#include "stdafx.h"
#include "ModelRender.h"


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
	//����������
	m_isInited = true;
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

	//����������
	m_isInited = true;
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
	int pos = (int)skeletonFilePath.find(".tkm");
	skeletonFilePath.replace(pos, 4, ".tks");

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
	//�������f�[�^���쐬����
	ModelInitData initData;
	//tkm�t�@�C���̃t�@�C���p�X���w�肷��B
	initData.m_tkmFilePath = filePath;
	//�V�F�[�_�[�t�@�C���̃t�@�C���p�X���w�肷��B
	initData.m_fxFilePath = "Assets/shader/PBR.fx";
	//�X�P���g�����w�肷��B
	if (m_skeletonPtr)	//�X�P���g��������������Ă�����
		initData.m_skeleton = m_skeletonPtr.get();
	//���f���̏�������w�肷��B
	initData.m_modelUpAxis = modelUpAxis;

	//�V�F�[�_�ɓn���R���X�^���g�o�b�t�@�̐ݒ�

	//���C�g���ʂ̃f�[�^�̓o�^
	initData.m_expandConstantBuffer =
		CLightManager::GetInstance()->GetLightParam();
	initData.m_expandConstantBufferSize =
		sizeof(*CLightManager::GetInstance()->GetLightParam());

	//�f�B���N�V�������C�g�B�̃f�[�^�̓o�^
	initData.m_expandConstantBuffer2 = 
		CLightManager::GetInstance()->GetDirectionLigData();
	initData.m_expandConstantBufferSize2 = 
		sizeof(*CLightManager::GetInstance()->GetDirectionLigData()) *
		CLightManager::GetMax_DirectionLight();

	//�|�C���g���C�g�B�̃f�[�^�̓o�^
	initData.m_expandConstantBuffer3 =
		CLightManager::GetInstance()->GetPointLigData();
	initData.m_expandConstantBufferSize3 =
		sizeof(*CLightManager::GetInstance()->GetPointLigData()) *
		CLightManager::GetMax_PointLight();

	//�X�|�b�g���C�g�B�̃f�[�^�̓o�^
	initData.m_expandConstantBuffer4 =
		CLightManager::GetInstance()->GetSpotLigData();
	initData.m_expandConstantBufferSize4 =
		sizeof(*CLightManager::GetInstance()->GetPointLigData()) *
		CLightManager::GetMax_SpotLight();

	//�V���h�E�̃f�[�^�̓o�^
	initData.m_shadowConstantBuffer =
		g_shadowMap->GetShadowParam();
	initData.m_shadowConstantBufferSize = 
		sizeof(*g_shadowMap->GetShadowParam()) * g_max_shadowMap;
	initData.m_expandShaderResoruceView = &g_shadowMap->GetShadowBlur();

	initData.m_cullMode = cullMode;

	//�쐬�����������f�[�^�����ƂɃ��f��������������A
	m_model.Init(initData);
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
	initShadowModelData.m_fxFilePath =
		"Assets/shader/DrawShadowMap.fx";
	//�V���h�E�̃p���[���[�^��萔�o�b�t�@�ɓn��
	initShadowModelData.m_expandConstantBuffer = (void*)g_shadowMap->GetShadowParam();
	initShadowModelData.m_expandConstantBufferSize = sizeof(*g_shadowMap->GetShadowParam());

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
		m_animationPtr->Progress(1.0f / 60.0f);
	//���f���̍��W�X�V
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);

	//�V���h�E�p�̃��f��������������Ă�����
	if (m_shadowModel.IsValid())
		//�p�����[�^�X�V
		m_shadowModel.UpdateModel(m_position, m_rotation, m_scale);

}


//�`��p�֐�
void CModelRender::Render(RenderContext& rc)
{
	//����������Ă��邩�H
	if (!m_isInited)
		return;	//����Ă��Ȃ��ꍇ�͉������Ȃ�

	//���f����`��
	m_model.Draw(rc);
}