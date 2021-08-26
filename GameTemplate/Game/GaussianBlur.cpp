#include "stdafx.h"
#include "GaussianBlur.h"

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

			//ガウシアンブラーの定数データを使用可能にする
			using namespace gaussianBlurConstData;

			/// <summary>
			/// 初期化。
			/// </summary>
			/// <param name="originalTexture">ガウシアンブラーをかけるオリジナルテクスチャ。</param>
			void CGaussianBlur::Init(Texture* originalTexture)
			{
				m_originalTexture = originalTexture;

				//レンダリングターゲットを初期化。
				InitRenderTargets();
				//スプライトを初期化。
				InitSprites();

				return;
			}

			/// <summary>
			/// ガウシアンブラーをGPU上で実行。
			/// </summary>
			/// <remarks>
			/// 本関数の呼び出しは、DirectX12を利用した描画コマンド生成中に呼び出す必要があります。
			/// </remarks>
			/// <param name="rc">レンダリングターゲット</param>
			/// <param name="blurPower">ブラーの強さ。値が大きいほどボケが強くなる。</param>
			void CGaussianBlur::ExecuteOnGPU(RenderContext& rc, float blurPower)
			{
				//重みテーブルを更新する。
				UpdateWeightsTable(blurPower);

				//横ブラーを実行。
				//レンダリングターゲットとして利用できるようになるまでwaitを入れる。
				rc.WaitUntilToPossibleSetRenderTarget(m_xBlurRenderTarget);
				//レンダリングターゲットを設定。
				rc.SetRenderTargetAndViewport(m_xBlurRenderTarget);
				//レンダリングターゲットをクリア。
				rc.ClearRenderTargetView(m_xBlurRenderTarget);

				//ドロー。
				m_xBlurSprite.Draw(rc);
				//レンダリングターゲットへの書き込み終了待ち。
				rc.WaitUntilFinishDrawingToRenderTarget(m_xBlurRenderTarget);

				//縦ブラーを実行。
				//レンダリングターゲットとして利用できるようになるまでwaitを入れる。
				rc.WaitUntilToPossibleSetRenderTarget(m_yBlurRenderTarget);
				//レンダリングターゲットを設定。
				rc.SetRenderTargetAndViewport(m_yBlurRenderTarget);
				//レンダリングターゲットをクリア。
				rc.ClearRenderTargetView(m_yBlurRenderTarget);

				//ドロー。
				m_yBlurSprite.Draw(rc);
				//レンダリングターゲットへの書き込み終了待ち。
				rc.WaitUntilFinishDrawingToRenderTarget(m_yBlurRenderTarget);

				return;
			}

			/// <summary>
			/// レンダリングターゲットを初期化。
			/// </summary>
			void CGaussianBlur::InitRenderTargets()
			{
				//Xブラー用のレンダリングターゲットを作成する。
				m_xBlurRenderTarget.Create(
					m_originalTexture->GetWidth() / 2,	//X方向の解像度を半分にする
					m_originalTexture->GetHeight(),
					1,
					1,
					m_originalTexture->GetFormat(),
					DXGI_FORMAT_D32_FLOAT
				);

				//Yブラー用のレンダリングターゲットを作成する。
				m_yBlurRenderTarget.Create(
					m_originalTexture->GetWidth() / 2,	//X方向と
					m_originalTexture->GetHeight() / 2,	//Y方向の解像度を半分にする
					1,
					1,
					m_originalTexture->GetFormat(),
					DXGI_FORMAT_D32_FLOAT
				);

				return;
			}

			/// <summary>
			/// スプライトを初期化。
			/// </summary>
			void CGaussianBlur::InitSprites()
			{
				//横ブラー用のスプライトを初期化する。
				{
					SpriteInitData xBlurSpriteInitData;
					xBlurSpriteInitData.m_fxFilePath = SHADER_FILEPATH_GAUSSIAN_BLUR;
					xBlurSpriteInitData.m_vsEntryPointFunc = ENTRY_POINT_FUNC_VS_X_BLUR;
					xBlurSpriteInitData.m_psEntryPoinFunc = ENTRY_POINT_FUNC_PS_BLUR;
					//スプライトの解像度はm_xBlurRenderTargetと同じ。
					xBlurSpriteInitData.m_width = m_xBlurRenderTarget.GetWidth();
					xBlurSpriteInitData.m_height = m_xBlurRenderTarget.GetHeight();
					//テクスチャは輝度テクスチャ。
					xBlurSpriteInitData.m_textures[0] = m_originalTexture;
					//描き込むレンダリングターゲットのフォーマットを指定する。
					xBlurSpriteInitData.m_colorBufferFormat = m_xBlurRenderTarget.GetColorBufferFormat();
					//ユーザー拡張の定数バッファにブラー用のパラメーターを設定する。
					xBlurSpriteInitData.m_expandConstantBuffer = &m_weights;
					xBlurSpriteInitData.m_expandConstantBufferSize = sizeof(m_weights);

					//初期化情報をもとに横ブラー用のスプライトを初期化する。
					m_xBlurSprite.Init(xBlurSpriteInitData);
				}
				//縦ブラー用のスプライトを初期化する。
				{
					SpriteInitData yBlurSpriteInitData;
					yBlurSpriteInitData.m_fxFilePath = SHADER_FILEPATH_GAUSSIAN_BLUR;
					yBlurSpriteInitData.m_vsEntryPointFunc = ENTRY_POINT_FUNC_VS_Y_BLUR;
					yBlurSpriteInitData.m_psEntryPoinFunc = ENTRY_POINT_FUNC_PS_BLUR;
					//スプライトの解像度はm_yBlurRenderTargetと同じ。
					yBlurSpriteInitData.m_width = m_yBlurRenderTarget.GetWidth();
					yBlurSpriteInitData.m_height = m_yBlurRenderTarget.GetHeight();
					//テクスチャは横ブラーをかけたもの。
					yBlurSpriteInitData.m_textures[0] = &m_xBlurRenderTarget.GetRenderTargetTexture();
					//描き込むレンダリングターゲットのフォーマットを指定する。
					yBlurSpriteInitData.m_colorBufferFormat = m_yBlurRenderTarget.GetColorBufferFormat();
					//ユーザー拡張の定数バッファにブラー用のパラメーターを設定する。
					yBlurSpriteInitData.m_expandConstantBuffer = &m_weights;
					yBlurSpriteInitData.m_expandConstantBufferSize = sizeof(m_weights);

					//初期化情報をもとに縦ブラー用のスプライトを初期化する。
					m_yBlurSprite.Init(yBlurSpriteInitData);
				}

				return;
			}

			/// <summary>
			/// 重みテーブルを更新する。
			/// </summary>
			void CGaussianBlur::UpdateWeightsTable(float blurPower)
			{
				float total = 0;
				for (int i = 0; i < NUM_WEIGHTS; i++) {
					m_weights[i] = expf(-0.5f * (float)(i * i) / blurPower);
					total += 2.0f * m_weights[i];

				}
				// 規格化
				for (int i = 0; i < NUM_WEIGHTS; i++) {
					m_weights[i] /= total;
				}

				return;
			}

		}
	}
}