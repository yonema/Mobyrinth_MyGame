#include "stdafx.h"
#include "ModelRender.h"
#include "LightManager.h"

void CModelRender::Init(const char* filePath)
{
	InitSkeleton(filePath);
	InitModel(filePath);
}

void CModelRender::InitSkeleton(const char* filePath)
{
	m_skeleton.Init("Assets/modelData/unityChan.tks");
}
void CModelRender::InitModel(const char* filePath)
{
	ModelInitData initData;
	//tkm�t�@�C���̃t�@�C���p�X���w�肷��B
	initData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	//�V�F�[�_�[�t�@�C���̃t�@�C���p�X���w�肷��B
	initData.m_fxFilePath = "Assets/shader/myModel.fx";
	//�m���X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
	initData.m_vsEntryPointFunc = "VSMain";
	//�X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w��B
	initData.m_vsSkinEntryPointFunc = "VSSkinMain";
	//�X�P���g�����w�肷��B
	initData.m_skeleton = &m_skeleton;
	//���f���̏�������w�肷��B
	//3dsMax�ł̓f�t�H���gZ�A�b�v�ɂȂ��Ă��邪�A
	//���j�e�B�����̓A�j���[�V������Y�A�b�v�ɕύX����Ă���B
	initData.m_modelUpAxis = enModelUpAxisY;


	initData.m_expandConstantBuffer = CLightManager::GetInstance()->GetLightParam();
	initData.m_expandConstantBufferSize = sizeof(*CLightManager::GetInstance()->GetLightParam());

	initData.m_expandConstantBuffer2 = CLightManager::GetInstance()->GetDirectionLigData();
	initData.m_expandConstantBufferSize2 = sizeof(*CLightManager::GetInstance()->GetDirectionLigData()) * 4;

	//�쐬�����������f�[�^�����ƂɃ��f��������������A
	m_model.Init(initData);

}