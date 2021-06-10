#pragma once
#include "LightManager.h"

/**
 * @brief CShadowMapを初期化するときの定数値
*/
namespace shadowConstData
{
	//影を作るライトの方向の初期値
	const Vector3 SHADOW_INIT_DIRECTION = { 1.0f,-1.0f,-1.0f };
	//影を作るライトの距離の初期値
	const float SHADOW_INIT_LENGTH = 1000.0f;
}

/// <summary>
/// シャドウマップクラス
/// </summary>
class CShadowMap
{
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
	/// 影を生成するライトを生成する
	/// </summary>
	/// <param name="direction">影を作るライトの方向</param>
	/// <param name="length">ライトがどれくらい離れているか</param>
	/// <param name="target">ライトが照らす目標</param>
	void CreateShadowMap
	(const Vector3& direction, const float length = 500.0f, const Vector3& target = g_VEC3_ZERO);

	/// <summary>
	/// 影を生成するライトのパラメーター設定する
	/// </summary>
	/// <param name="direction">影を作るライトの方向</param>
	/// <param name="length">ライトがどれくらい離れているか</param>
	/// <param name="target">ライトが照らす目標</param>
	void SetShadowParam
	(const Vector3& direction, const float length, const Vector3& target);

	/// <summary>
	/// シャドウのパラメータのポインタの初期化
	/// </summary>
	/// <returns>シャドウのパラメータのポインタ</returns>
	ShadowParam* GetShadowParam()
	{
		return m_shadowParam;
	}

	/// <summary>
	/// ガウシアンブラーのテクスチャの取得
	/// </summary>
	/// <returns>ガウシアンブラーのテクスチャ</returns>
	Texture& GetShadowBlur()
	{
		return m_gaussianBlur.GetBokeTexture();
	}

	/// <summary>
	/// シャドウマップに描画するシャドウ用モデルの登録
	/// </summary>
	/// <param name="shadowModel">登録するシャドウ用モデル</param>
	void AddShadowModel(Model& shadowModel);

	/// <summary>
	/// シャドウマップからシャドウ用モデルを破棄する
	/// </summary>
	/// <param name="shadowModel">破棄するシャドウ用モデル</param>
	void RemoveShadowModel(Model& shadowModel);

private:	//privateなメンバ関数

	/// <summary>
	/// シャドウマップのレンダーターゲットの初期化
	/// </summary>
	void InitShadowMapRenderTarget();

	/// <summary>
	/// ガウシアンブラーの初期化
	/// </summary>
	void InitGaussianBlur();

private:	//データメンバ

	ShadowParam m_shadowParam[g_max_shadowMap];	//シャドウのパラメータ
	RenderTarget m_shadowMapRenderTarget;		//シャドウマップのレンダーターゲット
	CGaussianBlur m_gaussianBlur;				//ガウシアンブラー
	std::list<Model*> m_shadowModels;			//シャドウマップに描画するシャドウ用モデルのリスト
	Vector3 m_targetPos = g_VEC3_ZERO;			//シャドウライトの注視点
};



