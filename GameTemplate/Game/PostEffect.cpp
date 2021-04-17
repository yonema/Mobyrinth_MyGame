#include "stdafx.h"
#include "PostEffect.h"

//コンストラクタ
CPostEffect::CPostEffect()
{

}

//デストラクタ
CPostEffect::~CPostEffect()
{

}

//初期化関数
void CPostEffect::Init()
{
	//ブルームを初期化する
	m_bloom.Init();
}

//描画する関数
void CPostEffect::Draw(RenderContext& renderContext)
{
	//メインレンダリングターゲット
	RenderTarget& mainRenderTarget = g_graphicsEngine->GetMainRenderTarget();

	// レンダリングターゲットへの書き込み終了待ち
	//renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);


	////////////////////////////////////////
	///	それぞれのポストエフェクトの処理実行
	////////////////////////////////////////


	//ブルームの処理
	m_bloom.Draw(renderContext);


	////////////////////////////////////////
	///	それぞれのポストエフェクトの処理終了
	////////////////////////////////////////


	//レンダリングターゲットをメインレンダリングターゲットに変更する
	renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	//メインレンダリングターゲットが利用できるまで待つ
	renderContext.SetRenderTargetAndViewport(mainRenderTarget);

	////////////////////////////////////////
	///	それぞれのポストエフェクトの結果をメインレンダリングターゲットに描画開始
	////////////////////////////////////////

	//ブルームをメインレンダリングターゲットに描画する
	m_bloom.DrawToMainRenderTarget(renderContext);


	////////////////////////////////////////
	///	それぞれのポストエフェクトの結果をメインレンダリングターゲットに描画終了
	////////////////////////////////////////

	//メインレンダリングターゲットへの書き込み終了待ち
	renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

}