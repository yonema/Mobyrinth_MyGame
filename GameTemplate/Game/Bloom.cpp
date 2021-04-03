#include "stdafx.h"
#include "Bloom.h"

CBloom::CBloom()
{

}

CBloom::~CBloom()
{

}

void CBloom::Init()
{
    //輝度抽出用のレンダリングターゲットを初期化
    InitluminanceRenderTarget();

    //輝度抽出用のスプライトを初期化
    InitluminanceSprite();

    //ガウシアンブラーの初期化
    InitGaussianBlur();
}

void CBloom::InitluminanceRenderTarget()
{
    //輝度抽出用のレンダリングターゲットを作成
    //解像度、ミップマップレベル
    m_luminanceRenderTarget.Create(
        1280,       // 解像度はメインレンダリングターゲットと同じ
        720,        // 解像度はメインレンダリングターゲットと同じ
        1,
        1,
        // 【注目】カラーバッファーのフォーマットを32bit浮動小数点にしている
        DXGI_FORMAT_R32G32B32A32_FLOAT,
        DXGI_FORMAT_D32_FLOAT
    );
}

void CBloom::InitluminanceSprite()
{
    // 輝度抽出用のスプライトを初期化
    // 初期化情報を作成する
    SpriteInitData luminanceSpriteInitData;

    // 輝度抽出用のシェーダーのファイルパスを指定する
    luminanceSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect.fx";

    // 頂点シェーダーのエントリーポイントを指定する
    luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";

    // ピクセルシェーダーのエントリーポイントを指定する
    luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";

    // スプライトの幅と高さはluminnceRenderTargetと同じ
    luminanceSpriteInitData.m_width = 1280;
    luminanceSpriteInitData.m_height = 720;

    // テクスチャはメインレンダリングターゲットのカラーバッファー
    luminanceSpriteInitData.m_textures[0] = 
        &g_graphicsEngine->GetMainRenderTarget().GetRenderTargetTexture();

    // 描き込むレンダリングターゲットのフォーマットを指定する
    luminanceSpriteInitData.m_colorBufferFormat = g_graphicsEngine->GetMainRenderTarget().GetColorBufferFormat();


    // 作成した初期化情報をもとにスプライトを初期化する
    m_luminanceSprite.Init(luminanceSpriteInitData);
}

void CBloom::InitGaussianBlur()
{
    m_gaussianBlur[0].Init(&m_luminanceRenderTarget.GetRenderTargetTexture());
    m_gaussianBlur[1].Init(&m_gaussianBlur[0].GetBokeTexture());
    m_gaussianBlur[2].Init(&m_gaussianBlur[1].GetBokeTexture());
    m_gaussianBlur[3].Init(&m_gaussianBlur[2].GetBokeTexture());

    SpriteInitData finalSpriteInitData;
    finalSpriteInitData.m_textures[0] = &m_gaussianBlur[0].GetBokeTexture();
    finalSpriteInitData.m_textures[1] = &m_gaussianBlur[1].GetBokeTexture();
    finalSpriteInitData.m_textures[2] = &m_gaussianBlur[2].GetBokeTexture();
    finalSpriteInitData.m_textures[3] = &m_gaussianBlur[3].GetBokeTexture();

    //ナゾ
    //後で先生に聞く
    finalSpriteInitData.m_width = 1280 * 4;
    finalSpriteInitData.m_height = 720 * 2;

    finalSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect.fx";
    finalSpriteInitData.m_psEntryPoinFunc = "PSBloomFinal";

    finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
    finalSpriteInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

    
    m_finalSprite.Init(finalSpriteInitData);
}

void CBloom::Draw(RenderContext& renderContext)
{
    // 輝度抽出
    // 輝度抽出用のレンダリングターゲットに変更
    renderContext.WaitUntilToPossibleSetRenderTarget(m_luminanceRenderTarget);

    // レンダリングターゲットを設定
    renderContext.SetRenderTargetAndViewport(m_luminanceRenderTarget);

    // レンダリングターゲットをクリア
    renderContext.ClearRenderTargetView(m_luminanceRenderTarget);


    // 輝度抽出を行う
    m_luminanceSprite.Draw(renderContext);

    // レンダリングターゲットへの書き込み終了待ち
    renderContext.WaitUntilFinishDrawingToRenderTarget(m_luminanceRenderTarget);

    //ガウシアンブラーを4回実行する
    for (int i = 0; i < 4; i++)
    {
        m_gaussianBlur[i].ExecuteOnGPU(renderContext, 10);
    }


}

void CBloom::DrawToMainRenderTarget(RenderContext& renderContext)
{
    m_finalSprite.Draw(renderContext);
}
