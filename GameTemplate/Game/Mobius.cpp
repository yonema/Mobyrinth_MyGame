#include "stdafx.h"
#include "Mobius.h"
#include "LevelObjectManager.h"

using namespace mobiusConstData;


//�X�^�[�g�֐�
bool CMobius::Start()
{
	//���f�������_���[�̐���
	m_modelRender = NewGO<CModelRender>(PRIORITY_FIRST);

	//�������f�[�^���쐬����
	ModelInitData initData;
	//tkm�t�@�C���̃t�@�C���p�X���w�肷��B
	initData.m_tkmFilePath = MODEL_FILEPATH_MOBIUS;
	//�V�F�[�_�[�t�@�C���̃t�@�C���p�X���w�肷��B
	initData.m_fxFilePath = SHADER_FILEPATH_MOBIUS;

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
	initData.m_expandShaderResoruceView[0] = &g_graphicsEngine->GetShadowMap().GetShadowBlur();

	//ZPrepass�ō쐬���ꂽ�[�x�e�N�X�`���̓o�^
	initData.m_expandShaderResoruceView[1] = &g_graphicsEngine->GetZPrepassDepthTexture();

	initData.m_expandShaderResoruceView[2] = &g_graphicsEngine->GetSkyCubeTexture();


	//���f�������_���[�̏�����
	m_modelRender->Init(initData);
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
	//m_modelRender->SetShadowCasterFlag(true);
	m_modelRender->SetShadowReceiverFlag(true);

	return true;
}

//�f�X�g���N�^
CMobius::~CMobius()
{
	//���f�������_���[�̔j��
	DeleteGO(m_modelRender);
	//�}�l�[�W���[�Ƀ��r�E�X�̗ւ��������ꂽ�Ɠ`����
	CLevelObjectManager::GetInstance()->SetMobius(nullptr);

	return;
}

/**
 * @brief ���r�E�X�̗ւƃ��C�̌����_�����߂�
 * @param [in] startPos ���C�̎n�_
 * @param [in] endPos ���C�̏I�_
 * @param [out] IntersectPos �����_
 * @return �������Ă��邩�H
*/
bool CMobius::GetIntersectPosWithMobius(
	const Vector3& startPos, const Vector3& endPos, Vector3* const IntersectPos
)
{
	//���r�E�X�̗ւ̃��f���ƃ��C���������Ă��邩�H
	if (m_modelRender->GetModel().InIntersectLine(startPos, endPos))
	{
		//��������

		//�����_�̍��W�������œn����Ă��邩�H
		if (IntersectPos)
			//�n����Ă���
			//�����_������
			*IntersectPos = m_modelRender->GetModel().GetIntersectPos();

		//���������A�Ɩ߂�
		return true;
	}

	//�������ĂȂ�
	return false;
}