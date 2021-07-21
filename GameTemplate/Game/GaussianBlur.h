#pragma once

/**
 * @brief ガウシアンブラーの定数データ
*/
namespace gaussianBlurConstData
{
	//重みの数
	constexpr int NUM_WEIGHTS = 8;

	//ガウシアンブラーのシェーダーファイルパス
	constexpr const char* const SHADER_FILEPATH_GAUSSIAN_BLUR = "Assets/shader/gaussianBlur.fx";

	//Xブラー用の頂点シェーダーのエントリーポイント
	constexpr const char* const ENTRY_POINT_FUNC_VS_X_BLUR = "VSXBlur";

	//Yブラー用の頂点シェーダーのエントリーポイント
	constexpr const char* const ENTRY_POINT_FUNC_VS_Y_BLUR = "VSYBlur";

	//ブラー用のピクセルシェーダーのエントリーポイント
	constexpr const char* const ENTRY_POINT_FUNC_PS_BLUR = "PSBlur";
}

/// <summary>
/// ガウシアンブラークラス。
/// </summary>
class CGaussianBlur : private Noncopyable
{
public:		//メンバ関数
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="originalTexture">ガウシアンブラーをかけるオリジナルテクスチャ。</param>
	void Init(Texture* originalTexture);

	/// <summary>
	/// ガウシアンブラーをGPU上で実行。
	/// </summary>
	/// <remarks>
	/// 本関数の呼び出しは、DirectX12を利用した描画コマンド生成中に呼び出す必要があります。
	/// </remarks>
	/// <param name="rc">レンダリングターゲット</param>
	/// <param name="blurPower">ブラーの強さ。値が大きいほどボケが強くなる。</param>
	void ExecuteOnGPU(RenderContext& rc, float blurPower);

	/// <summary>
	/// ボケテクスチャを取得。
	/// </summary>
	/// <returns></returns>
	Texture& GetBokeTexture()
	{
		return m_yBlurRenderTarget.GetRenderTargetTexture();
	}

private:	//privateなメンバ関数
	/// <summary>
	/// レンダリングターゲットを初期化。
	/// </summary>
	void InitRenderTargets();

	/// <summary>
	/// スプライトを初期化。
	/// </summary>
	void InitSprites();

	/// <summary>
	/// 重みテーブルを更新する。
	/// </summary>
	void UpdateWeightsTable(float blurPower);

private:	//データメンバ
	float m_weights[gaussianBlurConstData::NUM_WEIGHTS];			//重みテーブル。
	Texture* m_originalTexture = nullptr;	//オリジナルテクスチャ。
	RenderTarget m_xBlurRenderTarget;		//横ボケ画像を描画するレンダリングターゲット。
	RenderTarget m_yBlurRenderTarget;		//縦ボケ画像を描画するレンダリングターゲット。
	Sprite m_xBlurSprite;					//横ボケ画像を描画するためのスプライト。
	Sprite m_yBlurSprite;					//縦ボケ画像を描画するためのスプライト。
};

