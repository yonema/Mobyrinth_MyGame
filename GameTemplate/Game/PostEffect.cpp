#include "stdafx.h"
#include "PostEffect.h"


CPostEffect::CPostEffect()
{

}
CPostEffect::~CPostEffect()
{

}
void CPostEffect::Init()
{
	m_bloom.Init();
}
void CPostEffect::Draw(RenderContext& renderContext)
{
	//���C�������_�����O�^�[�Q�b�g
	RenderTarget& mainRenderTarget = g_graphicsEngine->GetMainRenderTarget();

	// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
	renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

	//�u���[���̏���
	m_bloom.Draw(renderContext);



	//�����_�����O�^�[�Q�b�g�����C�������_�����O�^�[�Q�b�g�ɕύX����
	renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	//���C�������_�����O�^�[�Q�b�g�����p�ł���܂ő҂�
	renderContext.SetRenderTargetAndViewport(mainRenderTarget);



	//�u���[�������C�������_�����O�^�[�Q�b�g�ɕ`�悷��
	m_bloom.DrawToMainRenderTarget(renderContext);




	//���C�������_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
	renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

}