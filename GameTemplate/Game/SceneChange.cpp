#include "stdafx.h"
#include "SceneChange.h"


void CSceneChange::Init()
{
	//レンダリングターゲットの初期化
	InitRenderTarget();

	InitSprite();

	srand((unsigned int)time(NULL));
}

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

void CSceneChange::InitSprite()
{
	//メインレンダリングターゲット
	RenderTarget& mainRenderTarget = g_graphicsEngine->GetMainRenderTarget();

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


	SpriteInitData finalSpriteInitData;
	//テクスチャをメインレンダーターゲットのテクスチャに設定
	finalSpriteInitData.m_textures[0] = &m_renderTarget.GetRenderTargetTexture();
	//シェーダーを設定
	finalSpriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
	//テクスチャの幅と高さを設定
	finalSpriteInitData.m_width = mainRenderTarget.GetWidth();
	finalSpriteInitData.m_height = mainRenderTarget.GetHeight();
	finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Trans;
	m_finalSprite.Init(finalSpriteInitData);


	SpriteInitData backSpriteInitData;
	backSpriteInitData.m_ddsFilePath[0] = "Assets/Image/backSprite.DDS";
	backSpriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
	backSpriteInitData.m_width = 1280.0f;
	backSpriteInitData.m_height = 720.0f;

	m_backSprite.Init(backSpriteInitData);
	m_backSprite.Update(g_vec3Zero, g_quatIdentity, { -1.0f,1.0f,1.0f });
}

void CSceneChange::UpdateParam()
{
	if (m_wipeFlag)
	{
		m_wipeParam.wipeSize += m_wipeSpeed;
	}
}

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
	if (m_wipeFlag)
		m_backSprite.Draw(renderContext);
	m_finalSprite.Draw(renderContext);

	//メインレンダリングターゲットへの書き込み終了待ち
	renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
}

void CSceneChange::SetWipeDirection(const Vector2& dir)
{
	m_wipeParam.wipeDir = dir;
	//スプライトは左右反転させているから、X軸を反転させる。
	m_wipeParam.wipeDir.x *= -1.0f;
	//正規化する
	m_wipeParam.wipeDir.Normalize();
}

void CSceneChange::RandomWipeStart()
{
	int randNum = rand() % 5;
	SetWipeType(randNum);
	if (randNum == enWipe)
		SetWipeSpeed(20.0f);
	else if (randNum == enCircleWipe)
		SetWipeSpeed(10.0f);
	else
		SetWipeSpeed(2.0f);

	WipeOut();
}

const float CSceneChange::GetWipeSizeMax(const int wipeType) const
{
	float sizeMax = m_wipeMaxX;

	switch (wipeType)
	{
	case enWipe:
		sizeMax = m_wipeMaxX;
		sizeMax += m_wipeMaxY * m_wipeParam.wipeDir.y;
		break;
	case enCircleWipe:
		sizeMax = m_circleWipeMax;
		break;
	case enVerticalStripeWipe:
		sizeMax = m_verticalStripeWipeMax;
		break;
	case enHorizontalStripeWipe:
		sizeMax = m_horizontalStripeWipeMax;
		break;
	case enCheckerboardWipe:
		sizeMax = m_checkerboardWipeMax;
		break;
	}

	return sizeMax;
}


const bool CSceneChange::IsWipeFinished() const
{
	bool isFinished = false;

	if (GetWipeSize() >= GetWipeSizeMax(GetWipeType()))
		isFinished = true;

	return isFinished;
}