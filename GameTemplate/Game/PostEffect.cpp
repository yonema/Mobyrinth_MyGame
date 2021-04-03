#include "stdafx.h"
#include "PostEffect.h"


CPostEffect::CPostEffect()
{

}
CPostEffect::~CPostEffect()
{

}
void CPostEffect::Init()
{
	m_bloom.Init();
}
void CPostEffect::Draw(RenderContext& renderContext)
{
	//メインレンダリングターゲット
	RenderTarget& mainRenderTarget = g_graphicsEngine->GetMainRenderTarget();

	// レンダリングターゲットへの書き込み終了待ち
	renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

	//ブルームの処理
	m_bloom.Draw(renderContext);



	//レンダリングターゲットをメインレンダリングターゲットに変更する
	renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	//メインレンダリングターゲットが利用できるまで待つ
	renderContext.SetRenderTargetAndViewport(mainRenderTarget);



	//ブルームをメインレンダリングターゲットに描画する
	m_bloom.DrawToMainRenderTarget(renderContext);




	//メインレンダリングターゲットへの書き込み終了待ち
	renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

}