#include "stdafx.h"
#include "SceneChange.h"
#include "GameTime.h"

//デストラクタ
CSceneChange::~CSceneChange()
{

}
//初期化関数
void CSceneChange::Init()
{
	//レンダリングターゲットの初期化
	InitRenderTarget();

	//スプライトの初期化
	InitSprite();

	//フェードの初期化
	InitFade();

	//乱数の初期化
	srand((unsigned int)time(NULL));
}

/// <summary>
/// レンダリングターゲットの初期化
/// </summary>
void CSceneChange::InitRenderTarget()
{
	//メインレンダリングターゲット
	RenderTarget& mainRenderTarget = g_graphicsEngine->GetMainRenderTarget();

	m_renderTarget.Create(
		mainRenderTarget.GetWidth(),
		mainRenderTarget.GetHeight(),
		1,
		1,
		// 【注目】カラーバッファーのフォーマットを32bit浮動小数点にしている
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);
}

/// <summary>
/// スプライトの初期化
/// </summary>
void CSceneChange::InitSprite()
{
	//メインレンダリングターゲット
	RenderTarget& mainRenderTarget = g_graphicsEngine->GetMainRenderTarget();

	//スプライトの初期化データ
	SpriteInitData spriteInitData;
	//テクスチャをメインレンダーターゲットのテクスチャに設定
	spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	//シェーダーを設定
	spriteInitData.m_fxFilePath = "Assets/shader/SceneChange.fx";
	//テクスチャの幅と高さを設定
	spriteInitData.m_width = mainRenderTarget.GetWidth();
	spriteInitData.m_height = mainRenderTarget.GetHeight();
	//定数バッファを設定
	spriteInitData.m_expandConstantBuffer = &m_wipeParam;
	spriteInitData.m_expandConstantBufferSize = sizeof(m_wipeParam);

	//スプライトの初期化
	m_sprite.Init(spriteInitData);


	//最終的なスプライトの初期化データ
	SpriteInitData finalSpriteInitData;
	//テクスチャをメインレンダーターゲットのテクスチャに設定
	finalSpriteInitData.m_textures[0] = &m_renderTarget.GetRenderTargetTexture();
	//シェーダーを設定
	finalSpriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
	//テクスチャの幅と高さを設定
	finalSpriteInitData.m_width = mainRenderTarget.GetWidth();
	finalSpriteInitData.m_height = mainRenderTarget.GetHeight();
	//アルファブレンドモード加算合成に設定
	finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

	//最終的なスプライトの初期化
	m_finalSprite.Init(finalSpriteInitData);


	//後ろに映すスプライトの初期化データ
	SpriteInitData backSpriteInitData;
	//テクスチャのファイルパスを設定
	backSpriteInitData.m_ddsFilePath[0] = "Assets/Image/backSprite2.DDS";
	//シェーダーを設定
	backSpriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
	//テクスチャの幅と高さを設定
	backSpriteInitData.m_width = 1280.0f;
	backSpriteInitData.m_height = 720.0f;

	//後ろに映すスプライトを初期化
	m_backSprite.Init(backSpriteInitData);
	//後ろに映すスプライトのアップデート
	m_backSprite.Update(g_VEC3_ZERO, g_QUAT_IDENTITY, { -1.0f,1.0f,1.0f });
}

/// <summary>
/// フェードの初期化
/// </summary>
void CSceneChange::InitFade()
{
	SpriteInitData fadeSpriteInitData;
	fadeSpriteInitData.m_ddsFilePath[0] = "Assets/Image/black.DDS";
	fadeSpriteInitData.m_width = 1280.0f;
	fadeSpriteInitData.m_height = 780.0f;
	fadeSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Trans;
	fadeSpriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

	m_fadeSprite.Init(fadeSpriteInitData);
	//透明にしておく
	m_fadeSprite.SetAlphaValue(0.0f);
}

//パラメータのアップデート関数
void CSceneChange::UpdateParam()
{
	//ワイプフラグが立っていたら
	if (m_wipeFlag)
	{
		//ワイプイン時は、直前にロードがあってデルタタイムが著しく長くなるから、
		//ワイプアウト時のデルタタイムを使う。
		if (m_wipeParam.inOrOut == enOut)
			m_deltaTime = GameTime().GetFrameDeltaTime();

		//ワイプを進める	//デルタタイムを掛ける
		m_wipeParam.wipeSize += m_wipeSpeed * m_deltaTime;
	}
}



//描画関数
void CSceneChange::Draw(RenderContext& renderContext)
{
	//メインレンダリングターゲット
	RenderTarget& mainRenderTarget = g_graphicsEngine->GetMainRenderTarget();

	//レンダリングターゲットを場面転換用のレンダリングターゲットに設定
	renderContext.WaitUntilToPossibleSetRenderTarget(m_renderTarget);

	// レンダリングターゲットを設定
	renderContext.SetRenderTargetAndViewport(m_renderTarget);

	// レンダリングターゲットをクリア
	renderContext.ClearRenderTargetView(m_renderTarget);


	m_sprite.Draw(renderContext);


	// レンダリングターゲットへの書き込み終了待ち
	renderContext.WaitUntilFinishDrawingToRenderTarget(m_renderTarget);



	//レンダリングターゲットをメインレンダリングターゲットに変更する
	renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	//メインレンダリングターゲットが利用できるまで待つ
	renderContext.SetRenderTargetAndViewport(mainRenderTarget);

	//ワイプしていたら、後ろのスプライトを描画する
	if (m_wipeFlag)
		m_backSprite.Draw(renderContext);

	m_finalSprite.Draw(renderContext);

	m_fadeSprite.Draw(renderContext);

	//メインレンダリングターゲットへの書き込み終了待ち
	renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
}

/// <summary>
/// ワイプする方向を設定する
/// leftOrLightと矛盾するように設定しないでね。
/// </summary>
/// <param name="dir">ワイプする方向</param>
void CSceneChange::SetWipeDirection(const Vector2& dir)
{
	m_wipeParam.wipeDir = dir;
	//スプライトは左右反転させているから、X軸を反転させる。
	m_wipeParam.wipeDir.x *= -1.0f;
	//正規化する
	m_wipeParam.wipeDir.Normalize();
}

/// <summary>
/// ワイプの種類をランダムにして、ワイプアウト開始
/// </summary>
void CSceneChange::RandomWipeOut()
{
	//0～4の5つの乱数を得る
	int randNum = rand() % 5;

	//ワイプの種類を設定する
	SetWipeType(randNum);

	//普通のワイプだったら
	if (randNum == enWipe)
		//速くワイプさせる
		SetWipeSpeed(1200.0f);
	//円形ワイプだったら
	else if (randNum == enCircleWipe)
		//ちょっとだけ早くワイプさせる
		SetWipeSpeed(600.0f);
	//その他だったら
	else
		//デフォルトのワイプスピードのする
		SetWipeSpeed(120.0f);

	//ワイプアウトする
	WipeOut();
}

/// <summary>
/// 各種類のワイプの最大サイズを得る
/// ワイプの種類はSceneChange.hを参照
/// </summary>
/// <param name="wipeType">ワイプの種類</param>
/// <returns>各種類のワイプの最大サイズ</returns>
const float CSceneChange::GetWipeSizeMax(const int wipeType) const
{
	//ワイプの最大のサイズ
	float sizeMax = m_wipeMaxX;

	//各種類ごとに割り振る
	switch (wipeType)
	{
	case enWipe:
		//普通のワイプ
		sizeMax = m_wipeMaxX;
		sizeMax += m_wipeMaxY * m_wipeParam.wipeDir.y;
		break;

	case enCircleWipe:
		//円形ワイプ
		sizeMax = m_circleWipeMax;
		break;

	case enVerticalStripeWipe:
		//縦縞ワイプ
		sizeMax = m_verticalStripeWipeMax;
		break;

	case enHorizontalStripeWipe:
		//横縞ワイプ
		sizeMax = m_horizontalStripeWipeMax;
		break;

	case enCheckerboardWipe:
		//チェッカーボードワイプ
		sizeMax = m_checkerboardWipeMax;
		break;
	}

	//ワイプの最大のサイズを戻す
	return sizeMax;
}

/// <summary>
/// ワイプが最後までワイプしたかを得る
/// </summary>
/// <returns>最後までワイプしたか？</returns>
const bool CSceneChange::IsWipeFinished() const
{
	//最後までワイプしたか？
	bool isFinished = !m_wipeFlag;

	//現在のワイプのサイズが、
	//現在のワイプの種類の最大のサイズより
	//大きかったら
	if (GetWipeSize() >= GetWipeSizeMax(GetWipeType()))
		//最後までワイプしている
		isFinished = true;

	//最後までワイプしたか戻す
	return isFinished;
}
