#include "stdafx.h"
#include "PostEffect.h"

//�R���X�g���N�^
CPostEffect::CPostEffect()
{

}

//�f�X�g���N�^
CPostEffect::~CPostEffect()
{

}

//�������֐�
void CPostEffect::Init()
{
	//�u���[��������������
	m_bloom.Init();
}

//�`�悷��֐�
void CPostEffect::Draw(RenderContext& renderContext)
{
	//���C�������_�����O�^�[�Q�b�g
	RenderTarget& mainRenderTarget = g_graphicsEngine->GetMainRenderTarget();

	// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
	//renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);


	////////////////////////////////////////
	///	���ꂼ��̃|�X�g�G�t�F�N�g�̏������s
	////////////////////////////////////////


	//�u���[���̏���
	m_bloom.Draw(renderContext);


	////////////////////////////////////////
	///	���ꂼ��̃|�X�g�G�t�F�N�g�̏����I��
	////////////////////////////////////////


	//�����_�����O�^�[�Q�b�g�����C�������_�����O�^�[�Q�b�g�ɕύX����
	renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	//���C�������_�����O�^�[�Q�b�g�����p�ł���܂ő҂�
	renderContext.SetRenderTargetAndViewport(mainRenderTarget);

	////////////////////////////////////////
	///	���ꂼ��̃|�X�g�G�t�F�N�g�̌��ʂ����C�������_�����O�^�[�Q�b�g�ɕ`��J�n
	////////////////////////////////////////

	//�u���[�������C�������_�����O�^�[�Q�b�g�ɕ`�悷��
	m_bloom.DrawToMainRenderTarget(renderContext);


	////////////////////////////////////////
	///	���ꂼ��̃|�X�g�G�t�F�N�g�̌��ʂ����C�������_�����O�^�[�Q�b�g�ɕ`��I��
	////////////////////////////////////////

	//���C�������_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
	renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

}