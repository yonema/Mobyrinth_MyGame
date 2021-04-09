#include "stdafx.h"
#include "Bloom.h"

//�R���X�g���N�^
CBloom::CBloom()
{

}

//�f�X�g���N�^
CBloom::~CBloom()
{

}

/// <summary>
/// �������֐�
/// </summary>
void CBloom::Init()
{
    //�P�x���o�p�̃����_�����O�^�[�Q�b�g��������
    InitluminanceRenderTarget();

    //�P�x���o�p�̃X�v���C�g��������
    InitluminanceSprite();

    //�K�E�V�A���u���[�̏�����
    InitGaussianBlur();
}

/// <summary>
/// �P�x���o�p�̃����_�����O�^�[�Q�b�g�̏�����
/// </summary>
void CBloom::InitluminanceRenderTarget()
{
    //�P�x���o�p�̃����_�����O�^�[�Q�b�g���쐬
    //�𑜓x�A�~�b�v�}�b�v���x��
    m_luminanceRenderTarget.Create(
        1280,       // �𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ���
        720,        // �𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ���
        1,
        1,
        // �y���ځz�J���[�o�b�t�@�[�̃t�H�[�}�b�g��32bit���������_�ɂ��Ă���
        DXGI_FORMAT_R32G32B32A32_FLOAT,
        DXGI_FORMAT_D32_FLOAT
    );
}

/// <summary>
/// �P�x���o�p�̃X�v���C�g�̏�����
/// </summary>
void CBloom::InitluminanceSprite()
{
    // �P�x���o�p�̃X�v���C�g��������
    // �����������쐬����
    SpriteInitData luminanceSpriteInitData;

    // �P�x���o�p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��
    luminanceSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect.fx";

    // ���_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��
    luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";

    // �s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g���w�肷��
    luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";

    // �X�v���C�g�̕��ƍ�����luminnceRenderTarget�Ɠ���
    luminanceSpriteInitData.m_width = 1280;
    luminanceSpriteInitData.m_height = 720;

    // �e�N�X�`���̓��C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�[
    luminanceSpriteInitData.m_textures[0] = 
        &g_graphicsEngine->GetMainRenderTarget().GetRenderTargetTexture();

    // �`�����ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w�肷��
    luminanceSpriteInitData.m_colorBufferFormat = g_graphicsEngine->GetMainRenderTarget().GetColorBufferFormat();


    // �쐬�����������������ƂɃX�v���C�g������������
    m_luminanceSprite.Init(luminanceSpriteInitData);
}

/// <summary>
/// �K�E�V�A���u���[�̏�����
/// </summary>
void CBloom::InitGaussianBlur()
{
    //�e�K�E�V�A���u���[�N���X�̏�����
    m_gaussianBlur[0].Init(&m_luminanceRenderTarget.GetRenderTargetTexture());
    m_gaussianBlur[1].Init(&m_gaussianBlur[0].GetBokeTexture());
    m_gaussianBlur[2].Init(&m_gaussianBlur[1].GetBokeTexture());
    m_gaussianBlur[3].Init(&m_gaussianBlur[2].GetBokeTexture());

    //�ŏI�I�ȃX�v���C�g�̏������f�[�^
    SpriteInitData finalSpriteInitData;

    //�e�K�E�V�A���u���[�̌��ʂ��e�N�X�`���ɐݒ�
    finalSpriteInitData.m_textures[0] = &m_gaussianBlur[0].GetBokeTexture();
    finalSpriteInitData.m_textures[1] = &m_gaussianBlur[1].GetBokeTexture();
    finalSpriteInitData.m_textures[2] = &m_gaussianBlur[2].GetBokeTexture();
    finalSpriteInitData.m_textures[3] = &m_gaussianBlur[3].GetBokeTexture();


    finalSpriteInitData.m_width = 1280;
    finalSpriteInitData.m_height = 720;

    finalSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect.fx";
    finalSpriteInitData.m_psEntryPoinFunc = "PSBloomFinal"; //�u���[���p�V�F�[�_�[

    finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;  //���Z����
    finalSpriteInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

    //�ŏI�I�ȃX�v���C�g�̏�����
    m_finalSprite.Init(finalSpriteInitData);
}

/// <summary>
/// �`��֐�
/// </summary>
/// <param name="renderContext">�����_�[�R���e�L�X�g</param>
void CBloom::Draw(RenderContext& renderContext)
{
    // �P�x���o
    // �P�x���o�p�̃����_�����O�^�[�Q�b�g�ɕύX
    renderContext.WaitUntilToPossibleSetRenderTarget(m_luminanceRenderTarget);

    // �����_�����O�^�[�Q�b�g��ݒ�
    renderContext.SetRenderTargetAndViewport(m_luminanceRenderTarget);

    // �����_�����O�^�[�Q�b�g���N���A
    renderContext.ClearRenderTargetView(m_luminanceRenderTarget);


    // �P�x���o���s��
    m_luminanceSprite.Draw(renderContext);

    // �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
    renderContext.WaitUntilFinishDrawingToRenderTarget(m_luminanceRenderTarget);

    //�K�E�V�A���u���[��4����s����
    for (int i = 0; i < 4; i++)
    {
        m_gaussianBlur[i].ExecuteOnGPU(renderContext, 10);
    }

}

/// <summary>
/// �u���[���̌��ʂ����C�������_�[�^�[�Q�b�g�ɕ`�悷��֐�
/// </summary>
/// <param name="renderContext">�����_�[�R���e�L�X�g</param>
void CBloom::DrawToMainRenderTarget(RenderContext& renderContext)
{
    //�ŏI�I�ȃX�v���C�g��`�悷��
    m_finalSprite.Draw(renderContext);
}
