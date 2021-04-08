#include "stdafx.h"
#include "ShadowMap.h"

void CShadowMap::Init()
{
	InitShadowMapRenderTarget();
	InitGaussianBlur();
}



void CShadowMap::InitShadowMapRenderTarget()
{
	//�V���h�E�}�b�v�`��p�̃����_�����O�^�[�Q�b�g���쐬����B
	float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
	m_shadowMapRenderTarget.Create(
		2048,
		2048,
		1,
		1,
		DXGI_FORMAT_R32G32_FLOAT,
		DXGI_FORMAT_D32_FLOAT,
		clearColor
	);
}

void CShadowMap::InitGaussianBlur()
{
	m_gaussianBlur.Init(&m_shadowMapRenderTarget.GetRenderTargetTexture());
}


void CShadowMap::CreateShadowMap(const Vector3& direction, const float length)
{
	if (CLightManager::GetInstance()->GetShadowNum() >= g_max_shadowMap)
		return;

	Vector3 dir = direction;
	dir.Normalize();
	Vector3 ligPos = g_vec3Zero - dir;
	ligPos.Scale(length);
	//�J�����̈ʒu��ݒ�B����̓��C�g�̈ʒu�B
	Camera lightCamera;
	lightCamera.SetPosition(ligPos);
	//�J�����̒����_��ݒ�B���ꂪ���C�g���Ƃ炵�Ă���ꏊ�B
	lightCamera.SetTarget(g_vec3Zero);
	//�������ݒ�B����̓��C�g���^���������Ă���̂ŁAX��������ɂ��Ă���B
	lightCamera.SetUp({1.0f,0.0f,0.0f});
	//lightCamera.SetUp(g_vec3Up);

	//���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă���B
	lightCamera.Update();


	m_shadowParam[CLightManager::GetInstance()->GetShadowNum()].lightPos = 
		lightCamera.GetPosition();
	m_shadowParam[CLightManager::GetInstance()->GetShadowNum()].mLVP =
		lightCamera.GetViewProjectionMatrix();

	CLightManager::GetInstance()->AddShadowNum();

}

void CShadowMap::Draw(RenderContext& renderContext)
{
	//�V���h�E�}�b�v�Ƀ����_�����O�B
	//�����_�����O�^�[�Q�b�g���V���h�E�}�b�v�ɕύX����B
	renderContext.WaitUntilToPossibleSetRenderTarget(m_shadowMapRenderTarget);
	
	renderContext.SetRenderTargetAndViewport(m_shadowMapRenderTarget);

	renderContext.ClearRenderTargetView(m_shadowMapRenderTarget);

	
	std::list<Model*>::iterator itr = m_shadowModels.begin();
	for (int shadowNum = 0; shadowNum < CLightManager::GetInstance()->GetShadowNum(); shadowNum++)
	{
		Camera lightCamera;
		lightCamera.SetPosition(m_shadowParam[shadowNum].lightPos);
		lightCamera.SetTarget(g_vec3Zero);
		lightCamera.SetUp({ 1.0f,0.0f,0.0f });
		//lightCamera.SetUp(g_vec3Up);
		lightCamera.Update();
		for (; itr != m_shadowModels.end(); itr++)
		{
			//�e���f����`��B
			(*itr)->Draw(renderContext, lightCamera.GetViewMatrix(), lightCamera.GetProjectionMatrix());
			//(*itr)->Draw(renderContext);
		}
	}

	//�������݊����҂��B
	renderContext.WaitUntilFinishDrawingToRenderTarget(m_shadowMapRenderTarget);


	//step-7 �V���h�E�}�b�v���ڂ������߂̃K�E�V�A���u���[�����s����B
	m_gaussianBlur.ExecuteOnGPU(renderContext, 5.0f);
}

void CShadowMap::AddShadow(Model& shadowModel)
{
	m_shadowModels.push_back(&shadowModel);
}
void CShadowMap::RemoveShadow(Model& shadowModel)
{
	std::list<Model*>::iterator itr = m_shadowModels.begin();
	for (; itr != m_shadowModels.end(); itr++)
	{
		if (*itr == &shadowModel)
		{
			m_shadowModels.erase(itr);
			break;
		}
	}
}