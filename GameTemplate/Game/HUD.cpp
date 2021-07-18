#include "stdafx.h"
#include "HUD.h"
#include "GameTime.h"

//HUDの定数データを使えるようにする
using namespace HUDConstData;

//コンストラクタ
CHUD::CHUD()
	:m_wipeSpeed(SPEED_WIPE_DEFAULT)	//ワイプのスピードをデフォルトのスピードにする
{
	//デルタタイムをセットしておく
	m_deltaTime = GameTime().GetFrameDeltaTime();

	return;
}

//初期化関数
void CHUD::Init()
{
	//レンダリングターゲットの初期化
	InitRenderTarget();

	//スプライトの初期化
	InitSprite();

	//フェードの初期化
	InitFade();

	//乱数の初期化
	srand((unsigned int)time(NULL));

	return;
}

/// <summary>
/// レンダリングターゲットの初期化
/// </summary>
void CHUD::InitRenderTarget()
{
	//メインレンダリングターゲット
	RenderTarget& mainRenderTarget = g_graphicsEngine->GetMainRenderTarget();
	
	//ワイプ前のレンダリングターゲットを作成
	m_beforeWipingRenderTarget.Create(
		mainRenderTarget.GetWidth(),	//メインレンダリングターゲットと同じにする
		mainRenderTarget.GetHeight(),	//メインレンダリングターゲットと同じにする
		1,
		1,
		// 【注目】カラーバッファーのフォーマットを32bit浮動小数点にしている
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);

	return;
}

/// <summary>
/// スプライトの初期化
/// </summary>
void CHUD::InitSprite()
{
	//メインレンダリングターゲット
	RenderTarget& mainRenderTarget = g_graphicsEngine->GetMainRenderTarget();

	//メインレンダリングターゲットのスプライトの初期化データ
	SpriteInitData mainRenderTargetSpriteInitData;
	//テクスチャをメインレンダーターゲットのテクスチャに設定
	mainRenderTargetSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	//シェーダーを設定
	mainRenderTargetSpriteInitData.m_fxFilePath = SHADER_FILEPATH_WIPE;
	//テクスチャの幅と高さを設定
	mainRenderTargetSpriteInitData.m_width = mainRenderTarget.GetWidth();
	mainRenderTargetSpriteInitData.m_height = mainRenderTarget.GetHeight();
	//定数バッファを設定
	mainRenderTargetSpriteInitData.m_expandConstantBuffer = &m_wipeParam;
	mainRenderTargetSpriteInitData.m_expandConstantBufferSize = sizeof(m_wipeParam);

	//メインレンダリングターゲットのスプライトの初期化
	m_mainRenderTargetSprite.Init(mainRenderTargetSpriteInitData);


	//ワイプした後のスプライトの初期化データ
	SpriteInitData afterWipingSpriteInitData;
	//テクスチャをメインレンダーターゲットのテクスチャに設定
	afterWipingSpriteInitData.m_textures[0] = &m_beforeWipingRenderTarget.GetRenderTargetTexture();
	//シェーダーを設定
	afterWipingSpriteInitData.m_fxFilePath = spriteRenderConstData::SHADER_FILEPATH_DEFAULT;
	//テクスチャの幅と高さを設定
	afterWipingSpriteInitData.m_width = mainRenderTarget.GetWidth();
	afterWipingSpriteInitData.m_height = mainRenderTarget.GetHeight();
	//アルファブレンドモードを透過合成に設定
	afterWipingSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

	//ワイプした後のスプライトの初期化
	m_afterWipingSprite.Init(afterWipingSpriteInitData);


	//後ろに映すスプライトの初期化データ
	SpriteInitData backSpriteInitData;
	//テクスチャのファイルパスを設定
	backSpriteInitData.m_ddsFilePath[0] = SPRITE_FILEPATH_BACK_SPRITE;
	//シェーダーを設定
	backSpriteInitData.m_fxFilePath = spriteRenderConstData::SHADER_FILEPATH_DEFAULT;
	//テクスチャの幅と高さを設定
	backSpriteInitData.m_width = mainRenderTarget.GetWidth();;
	backSpriteInitData.m_height = mainRenderTarget.GetHeight();;

	//後ろに映すスプライトを初期化
	m_backSprite.Init(backSpriteInitData);
	//後ろに映すスプライトのアップデート
	m_backSprite.Update(
		g_VEC3_ZERO, g_QUAT_IDENTITY,
		spriteRenderConstData::SPRITE_SCALE_DEFAULT
	);

	return;
}

/// <summary>
/// フェードの初期化
/// </summary>
void CHUD::InitFade()
{
	//フェード用のスプライトの初期化データ
	SpriteInitData fadeSpriteInitData;
	fadeSpriteInitData.m_ddsFilePath[0] = SPRITE_FILEPATH_FADE;	//ddsのファイスパス
	fadeSpriteInitData.m_width = g_FRAME_BUFFER_W;				//横幅
	fadeSpriteInitData.m_height = g_FRAME_BUFFER_H;				//縦幅
	fadeSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Trans;	//透過合成モード
	//シェーダーのファイルパス
	fadeSpriteInitData.m_fxFilePath = spriteRenderConstData::SHADER_FILEPATH_DEFAULT;

	//フェード用のスプライトの初期化
	m_fadeSprite.Init(fadeSpriteInitData);

	//透明にしておく
	m_fadeSprite.SetAlphaValue(spriteRenderConstData::ALPHA_VALUE_TRANSPARENT);

	return;
}

//パラメータのアップデート関数
void CHUD::UpdateParam()
{
	//ワイプフラグが立っていたら
	if (m_wipeFlag)
	{
		//ワイプイン時は、直前にロードがあってデルタタイムが著しく長くなるから、
		//ワイプアウト時のデルタタイムを使う。
		if (m_wipeParam.inOrOut == EN_OUT)
			m_deltaTime = GameTime().GetFrameDeltaTime();

		//ワイプを進める	//デルタタイムを掛ける
		m_wipeParam.wipeSize += m_wipeSpeed * m_deltaTime;
	}

	return;
}



//描画関数
void CHUD::Draw(RenderContext& renderContext)
{
	//メインレンダリングターゲット
	RenderTarget& mainRenderTarget = g_graphicsEngine->GetMainRenderTarget();

	//ワイプ前のレンダリングターゲットが利用可能になるまで待つ
	renderContext.WaitUntilToPossibleSetRenderTarget(m_beforeWipingRenderTarget);
	// レンダリングターゲットを設定
	renderContext.SetRenderTargetAndViewport(m_beforeWipingRenderTarget);
	// レンダリングターゲットをクリア
	renderContext.ClearRenderTargetView(m_beforeWipingRenderTarget);

	//メインレンダリングターゲットのスプライトを描き込む
	m_mainRenderTargetSprite.Draw(renderContext);

	// レンダリングターゲットへの描き込み終了待ち
	renderContext.WaitUntilFinishDrawingToRenderTarget(m_beforeWipingRenderTarget);



	//レンダリングターゲットをメインレンダリングターゲットに変更する
	renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	//メインレンダリングターゲットが利用できるまで待つ
	renderContext.SetRenderTargetAndViewport(mainRenderTarget);

	//ワイプしていたら、後ろのスプライトを描画する
	if (m_wipeFlag)
		m_backSprite.Draw(renderContext);

	//ワイプ後のスプライトの描画する
	m_afterWipingSprite.Draw(renderContext);


	//フェードのスプライトを描画する
	m_fadeSprite.Draw(renderContext);

	//メインレンダリングターゲットへの書き込み終了待ち
	renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

	return;
}

/// <summary>
/// ワイプする方向を設定する
/// leftOrLightと矛盾するように設定しないでね。
/// </summary>
/// <param name="dir">ワイプする方向</param>
void CHUD::SetWipeDirection(const Vector2& dir)
{
	//ワイプの方向を設定する
	m_wipeParam.wipeDir = dir;
	//スプライトは左右反転させているから、X軸を反転させる。
	m_wipeParam.wipeDir.x *= -1.0f;
	//正規化する
	m_wipeParam.wipeDir.Normalize();

	return;
}

/// <summary>
/// ワイプの種類をランダムにして、ワイプアウト開始
/// </summary>
void CHUD::RandomWipeOut()
{
	//0～4の5つの乱数を得る
	int randNum = rand() % 5;
	randNum = 0;
	//ワイプの種類を設定する
	SetWipeType(randNum);

	//普通のワイプだったら
	if (randNum == EN_WIPE)
		//速くワイプさせる
		SetWipeSpeed(SPEED_WIPE);
	//円形ワイプだったら
	else if (randNum == EN_CIRCLE_WIPE)
		//ちょっとだけ早くワイプさせる
		SetWipeSpeed(SPEED_WIPE_CIRLE);
	//その他だったら
	else
		//デフォルトのワイプスピードのする
		SetWipeSpeed(SPEED_WIPE_DEFAULT);

	//ワイプアウトする
	WipeOut();

	return;
}

/// <summary>
/// 各種類のワイプの最大サイズを得る
/// ワイプの種類はSceneChange.hを参照
/// </summary>
/// <param name="wipeType">ワイプの種類</param>
/// <returns>各種類のワイプの最大サイズ</returns>
const float CHUD::GetWipeSizeMax(const int wipeType) const
{
	//ワイプの最大のサイズ
	float sizeMax = SIZE_MAX_X_WIPE;

	//各種類ごとに割り振る
	switch (wipeType)
	{
	case EN_WIPE:
		//普通のワイプ
		sizeMax = SIZE_MAX_X_WIPE;
		sizeMax += SIZE_MAX_Y_WIPE * m_wipeParam.wipeDir.y;
		break;

	case EN_CIRCLE_WIPE:
		//円形ワイプ
		sizeMax = SIZE_MAX_WIPE_CIRCLE;
		break;

	case EN_VERTICAL_STRIPE_WIPE:
		//縦縞ワイプ
		sizeMax = SIZE_MAX_WIPE_STRIPE_VERTICAL;
		break;

	case EN_HORIZONTAL_STRIPE_WIPE:
		//横縞ワイプ
		sizeMax = SIZE_MAX_WIPE_STRIPE_HORIZONTAL;
		break;

	case EN_CHECKERBOARD_WIPE:
		//チェッカーボードワイプ
		sizeMax = SIZE_MAX_WIPE_CHECKERBOARD;
		break;
	}

	//ワイプの最大のサイズを戻す
	return sizeMax;
}

/// <summary>
/// ワイプが最後までワイプしたかを得る
/// </summary>
/// <returns>最後までワイプしたか？</returns>
const bool CHUD::IsWipeFinished() const
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
