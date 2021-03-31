#include "stdafx.h"
#include "Bloom.h"

CBloom::CBloom()
{
    //輝度抽出用のレンダリングターゲットを初期化
    InitluminanceRenderTarget();
    //輝度抽出するレンダリングターゲットをテクスチャとして保持する
    InitRenderTargetTextrue();
    //輝度抽出用のスプライトを初期化
    InitluminanceSprite();
}

CBloom::~CBloom()
{

}

void CBloom::InitluminanceRenderTarget()
{
    //輝度抽出用のレンダリングターゲットを作成
    //解像度、ミップマップレベル
    luminanceRenderTarget.Create(
        1280,       // 解像度はメインレンダリングターゲットと同じ
        720,        // 解像度はメインレンダリングターゲットと同じ
        1,
        1,
        // 【注目】カラーバッファーのフォーマットを32bit浮動小数点にしている
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
    // 輝度抽出用のスプライトを初期化
    // 初期化情報を作成する
    SpriteInitData luminanceSpriteInitData;

    // 輝度抽出用のシェーダーのファイルパスを指定する
    luminanceSpriteInitData.m_fxFilePath = "Assets/shader/samplePostEffect.fx";

    // 頂点シェーダーのエントリーポイントを指定する
    luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";

    // ピクセルシェーダーのエントリーポイントを指定する
    luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";

    // スプライトの幅と高さはluminnceRenderTargetと同じ
    luminanceSpriteInitData.m_width = 1280;
    luminanceSpriteInitData.m_height = 720;

    // テクスチャはメインレンダリングターゲットのカラーバッファー
    luminanceSpriteInitData.m_textures[0] = &m_renderTargetTextrue;

    // 描き込むレンダリングターゲットのフォーマットを指定する
    luminanceSpriteInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;


    // 作成した初期化情報をもとにスプライトを初期化する
    m_luminanceSprite.Init(luminanceSpriteInitData);
}