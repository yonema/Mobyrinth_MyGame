#include "stdafx.h"
#include "ModelRender.h"
#include "LightManager.h"

void CModelRender::Init(
	const char* filePath,
	AnimationClip* animationClips,
	int numAnimationClips,
	EnModelUpAxis modelUpAxis)
{
	//�X�P���g���̃f�[�^�̓ǂݍ��݁B
	InitSkeleton(filePath);
	//���f����������
	InitModel(filePath, modelUpAxis);
	//�A�j���[�V������������
	InitAnimation(animationClips, numAnimationClips);

	//����������
	m_isInited = true;
}
void CModelRender::Init(
	ModelInitData initData,
	AnimationClip* animationClips ,
	int numAnimationClips)
{
	//�X�P���g���̃f�[�^�̓ǂݍ��݁B
	InitSkeleton(initData.m_tkmFilePath);
	//���f����������
	m_model.Init(initData);
	//�A�j���[�V������������
	InitAnimation(animationClips, numAnimationClips);

	//����������
	m_isInited = true;
}

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
void CModelRender::InitModel(const char* filePath, EnModelUpAxis modelUpAxis)
{
	//�������f�[�^���쐬����
	ModelInitData initData;
	//tkm�t�@�C���̃t�@�C���p�X���w�肷��B
	initData.m_tkmFilePath = filePath;
	//�V�F�[�_�[�t�@�C���̃t�@�C���p�X���w�肷��B
	initData.m_fxFilePath = "Assets/shader/myModel.fx";
	//�X�P���g�����w�肷��B
	if (m_skeletonPtr)	//�X�P���g��������������Ă�����
		initData.m_skeleton = m_skeletonPtr.get();
	//���f���̏�������w�肷��B
	initData.m_modelUpAxis = modelUpAxis;

	//�V�F�[�_�ɓn���R���X�^���g�o�b�t�@�̐ݒ�

	//���C�g���ʂ̃f�[�^�̓o�^
	initData.m_expandConstantBuffer
		= CLightManager::GetInstance()->GetLightParam();
	initData.m_expandConstantBufferSize
		= sizeof(*CLightManager::GetInstance()->GetLightParam());

	//�f�B���N�V�������C�g�B�̃f�[�^�̓o�^
	initData.m_expandConstantBuffer2
		= CLightManager::GetInstance()->GetDirectionLigData();
	initData.m_expandConstantBufferSize2
		= sizeof(*CLightManager::GetInstance()->GetDirectionLigData())
		* CLightManager::GetMax_DirectionLight();


	//�쐬�����������f�[�^�����ƂɃ��f��������������A
	m_model.Init(initData);
}
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

}

void CModelRender::Render(RenderContext& rc)
{
	//����������Ă��邩�H
	if (!m_isInited)
		return;	//����Ă��Ȃ��ꍇ�͉������Ȃ�

	//���f����`��
	m_model.Draw(rc);
}