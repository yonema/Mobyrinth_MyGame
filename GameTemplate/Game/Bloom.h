#pragma once
#include "GaussianBlur.h"
#include "SpriteRenderConstData.h"

/**
 * @brief ブルームの定数データ
*/
namespace BloomConstData
{
	//ガウシアンブラーの数
	constexpr int GAUSSIAN_BLUR_NUM = 4;

	//ガウシアンブラーの強さ
	constexpr int POWER_GAUSSIAN_BLUR = 10;

	//ポストエフェクトのシェーダーファイルパス
	constexpr const char* const SHADER_FILEPATH_POST_EFFECT = "Assets/shader/PostEffect.fx";

	//輝度抽出用スプライト用のピクセルシェーダー
	constexpr const char* const ENTRY_POINT_FUNC_PS_LUMINANCE_SPRITE = "PSSamplingLuminance";

	//ブルーム用のピクセルシェーダー
	constexpr const char* const ENTRY_POINT_FUNC_PS_BLOOM = "PSBloomFinal";
}

class CBloom
{
public:		//自動で呼ばれるメンバ関数
	CBloom();		//コンストラクタ
	~CBloom();		//デストラクタ

public:		//メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 描画関数
	/// </summary>
	/// <param name="renderContext">レンダーコンテキスト</param>
	void Draw(RenderContext& renderContext);

	/// <summary>
	/// ブルームの結果をメインレンダーターゲットに描画する関数
	/// </summary>
	/// <param name="renderContext">レンダーコンテキスト</param>
	void DrawToMainRenderTarget(RenderContext& renderContext);

private:	//privateなメンバ関数

	/// <summary>
	/// 輝度抽出用のレンダリングターゲットの初期化
	/// </summary>
	void InitluminanceRenderTarget();

	/// <summary>
	/// 輝度抽出用のスプライトの初期化
	/// </summary>
	void InitluminanceSprite();

	/// <summary>
	/// ガウシアンブラーの初期化
	/// </summary>
	void InitGaussianBlur();

private:	//データメンバ
	RenderTarget m_luminanceRenderTarget;	//輝度抽出用のレンダリングターゲット
	Sprite m_luminanceSprite;				//輝度抽出用のスプライト
	CGaussianBlur m_gaussianBlur[BloomConstData::GAUSSIAN_BLUR_NUM];	//ガウシアンブラー４枚
	Sprite m_finalSprite;					//最終的なスプライト

};

