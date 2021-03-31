#include "stdafx.h"
#include "Bloom.h"

CBloom::CBloom()
{
    //�P�x���o�p�̃����_�����O�^�[�Q�b�g��������
    InitluminanceRenderTarget();
    //�P�x���o���郌���_�����O�^�[�Q�b�g���e�N�X�`���Ƃ��ĕێ�����
    InitRenderTargetTextrue();
    //�P�x���o�p�̃X�v���C�g��������
    InitluminanceSprite();
}

CBloom::~CBloom()
{

}

void CBloom::InitluminanceRenderTarget()
{
    //�P�x���o�p�̃����_�����O�^�[�Q�b�g���쐬
    //�𑜓x�A�~�b�v�}�b�v���x��
    luminanceRenderTarget.Create(
        1280,       // �𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ���
        720,        // �𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ���
        1,
        1,
        // �y���ځz�J���[�o�b�t�@�[�̃t�H�[�}�b�g��32bit���������_�ɂ��Ă���
        DXGI_FORMAT_R32G32B32A32_FLOAT,
        DXGI_FORMAT_D32_FLOAT
    );
}
void CBloom::InitRenderTargetTextrue()
{
    m_renderTargetTextrue.InitFromD3DResource(g_graphicsEngine->GetRenderTarget());
}

void CBloom::InitluminanceSprite()
{
    // �P�x���o�p�̃X�v���C�g��������
    // �����������쐬����
    SpriteInitData luminanceSpriteInitData;

    // �P�x���o�p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��
    luminanceSpriteInitData.m_fxFilePath = "Assets/shader/samplePostEffect.fx";

    // ���_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��
    luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";

    // �s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g���w�肷��
    luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";

    // �X�v���C�g�̕��ƍ�����luminnceRenderTarget�Ɠ���
    luminanceSpriteInitData.m_width = 1280;
    luminanceSpriteInitData.m_height = 720;

    // �e�N�X�`���̓��C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�[
    luminanceSpriteInitData.m_textures[0] = &m_renderTargetTextrue;

    // �`�����ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w�肷��
    luminanceSpriteInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;


    // �쐬�����������������ƂɃX�v���C�g������������
    m_luminanceSprite.Init(luminanceSpriteInitData);
}