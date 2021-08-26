#include "stdafx.h"
#include "Bloom.h"

/**
 * @brief メビリンス
*/
namespace nsMobyrinth
{
    /**
     * @brief グラフィック
    */
    namespace nsGraphic
    {
        /**
         * @brief ポストエフェクト
        */
        namespace nsPostEffect
        {

            //ブルームの定数データを使用可能にする
            using namespace bloomConstData;

            //コンストラクタ
            CBloom::CBloom()
            {

            }

            //デストラクタ
            CBloom::~CBloom()
            {

            }

            /// <summary>
            /// 初期化関数
            /// </summary>
            void CBloom::Init()
            {
                //輝度抽出用のレンダリングターゲットを初期化
                InitluminanceRenderTarget();

                //輝度抽出用のスプライトを初期化
                InitluminanceSprite();

                //ガウシアンブラーの初期化
                InitGaussianBlur();

                return;
            }

            /// <summary>
            /// 輝度抽出用のレンダリングターゲットの初期化
            /// </summary>
            void CBloom::InitluminanceRenderTarget()
            {
                //輝度抽出用のレンダリングターゲットを作成
                //解像度、ミップマップレベル
                m_luminanceRenderTarget.Create(
                    g_FRAME_BUFFER_W,       // 解像度はメインレンダリングターゲットと同じ
                    g_FRAME_BUFFER_H,        // 解像度はメインレンダリングターゲットと同じ
                    1,
                    1,
                    // 【注目】カラーバッファーのフォーマットを32bit浮動小数点にしている
                    DXGI_FORMAT_R32G32B32A32_FLOAT,
                    DXGI_FORMAT_D32_FLOAT
                );

                return;
            }

            /// <summary>
            /// 輝度抽出用のスプライトの初期化
            /// </summary>
            void CBloom::InitluminanceSprite()
            {
                // 輝度抽出用のスプライトを初期化
                // 初期化情報を作成する
                SpriteInitData luminanceSpriteInitData;

                // 輝度抽出用のシェーダーのファイルパスを指定する
                luminanceSpriteInitData.m_fxFilePath = SHADER_FILEPATH_POST_EFFECT;

                // 頂点シェーダーのエントリーポイントを指定する
                luminanceSpriteInitData.m_vsEntryPointFunc = nsSprite::spriteRenderConstData::ENTRY_POINT_FUNC_VS_DEFAULT;

                // ピクセルシェーダーのエントリーポイントを指定する
                luminanceSpriteInitData.m_psEntryPoinFunc = ENTRY_POINT_FUNC_PS_LUMINANCE_SPRITE;

                // スプライトの幅と高さはluminnceRenderTargetと同じ
                luminanceSpriteInitData.m_width = g_FRAME_BUFFER_W;
                luminanceSpriteInitData.m_height = g_FRAME_BUFFER_H;

                // テクスチャはメインレンダリングターゲットのカラーバッファー
                luminanceSpriteInitData.m_textures[0] =
                    &g_graphicsEngine->GetMainRenderTarget().GetRenderTargetTexture();

                // 描き込むレンダリングターゲットのフォーマットを指定する
                luminanceSpriteInitData.m_colorBufferFormat = g_graphicsEngine->GetMainRenderTarget().GetColorBufferFormat();


                // 作成した初期化情報をもとにスプライトを初期化する
                m_luminanceSprite.Init(luminanceSpriteInitData);

                return;
            }

            /// <summary>
            /// ガウシアンブラーの初期化
            /// </summary>
            void CBloom::InitGaussianBlur()
            {
                //各ガウシアンブラークラスの初期化
                m_gaussianBlur[0].Init(&m_luminanceRenderTarget.GetRenderTargetTexture());
                m_gaussianBlur[1].Init(&m_gaussianBlur[0].GetBokeTexture());
                m_gaussianBlur[2].Init(&m_gaussianBlur[1].GetBokeTexture());
                m_gaussianBlur[3].Init(&m_gaussianBlur[2].GetBokeTexture());

                //最終的なスプライトの初期化データ
                SpriteInitData finalSpriteInitData;

                //各ガウシアンブラーの結果をテクスチャに設定
                finalSpriteInitData.m_textures[0] = &m_gaussianBlur[0].GetBokeTexture();
                finalSpriteInitData.m_textures[1] = &m_gaussianBlur[1].GetBokeTexture();
                finalSpriteInitData.m_textures[2] = &m_gaussianBlur[2].GetBokeTexture();
                finalSpriteInitData.m_textures[3] = &m_gaussianBlur[3].GetBokeTexture();


                finalSpriteInitData.m_width = g_FRAME_BUFFER_W;
                finalSpriteInitData.m_height = g_FRAME_BUFFER_H;

                finalSpriteInitData.m_fxFilePath = SHADER_FILEPATH_POST_EFFECT;
                finalSpriteInitData.m_psEntryPoinFunc = ENTRY_POINT_FUNC_PS_BLOOM; //ブルーム用シェーダー

                finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;  //加算合成
                finalSpriteInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

                //最終的なスプライトの初期化
                m_finalSprite.Init(finalSpriteInitData);

                return;
            }

            /// <summary>
            /// 描画関数
            /// </summary>
            /// <param name="renderContext">レンダーコンテキスト</param>
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
                for (int i = 0; i < GAUSSIAN_BLUR_NUM; i++)
                {
                    m_gaussianBlur[i].ExecuteOnGPU(renderContext, POWER_GAUSSIAN_BLUR);
                }

                return;
            }

            /// <summary>
            /// ブルームの結果をメインレンダーターゲットに描画する関数
            /// </summary>
            /// <param name="renderContext">レンダーコンテキスト</param>
            void CBloom::DrawToMainRenderTarget(RenderContext& renderContext)
            {
                //最終的なスプライトを描画する
                m_finalSprite.Draw(renderContext);

                return;
            }
        }
    }
}