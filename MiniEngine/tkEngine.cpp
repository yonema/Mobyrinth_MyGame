#include "stdafx.h"
#include "tkEngine.h"
#include "GraphicsEngine.h"



TkEngine* g_engine = nullptr;

TkEngine::~TkEngine()
{
	if (m_graphicsEngine) {
		delete m_graphicsEngine;
	}
}
void TkEngine::Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight)
{
	//グラフィックエンジンの初期化。
	m_graphicsEngine = new GraphicsEngine();
	m_graphicsEngine->Init(hwnd, frameBufferWidth, frameBufferHeight);
	//ゲームパッドの初期化。
	for (int i = 0; i < GamePad::CONNECT_PAD_MAX; i++) {
		g_pad[i] = &m_pad[i];
	}
}
void TkEngine::BeginFrame()
{
	m_graphicsEngine->BeginRender();
	for (auto& pad : m_pad) {
		pad.BeginFrame();
		pad.Update();
	}
	
}
void TkEngine::EndFrame()
{
	m_graphicsEngine->EndRender();
}


//追加


/// <summary>
/// シャドウを描画する
/// </summary>
void TkEngine::DrawShadow()
{
	m_graphicsEngine->ShadowRender();
}

/// <summary>
/// メインレンダーターゲットを使用できるようにする
/// </summary>
void TkEngine::UseMainRenderTarget()
{
	g_graphicsEngine->UseMainRenderTarget();
}

/// <summary>
/// ポストエフェクトを描画する
/// </summary>
void TkEngine::DrawPostEffect()
{
	m_graphicsEngine->PostEffectRender();
}

/// <summary>
/// メインレンダリングターゲットの絵をフレームバッファーに描画する
/// </summary>	
void TkEngine::DrawFrameBuffer()
{
	m_graphicsEngine->CopyToFrameBuffer();
}

/// <summary>
/// 影を生成するライトを生成する
/// </summary>
/// <param name="direction">影を作るライトの方向</param>
/// <param name="length">ライトがどれくらい離れているか</param>
/// <param name="target">ライトが照らす目標</param>
void TkEngine::CreateShadow(const Vector3& direction, const float length, const Vector3& target)
{
	m_graphicsEngine->CreateShadowMap(direction, length, target);
}

/// <summary>
/// 影を生成するライトのパラメーター設定する
/// </summary>
/// <param name="direction">影を作るライトの方向</param>
/// <param name="length">ライトがどれくらい離れているか</param>
/// <param name="target">ライトが照らす目標</param>
void TkEngine::SetShadowPararm(const Vector3& direction, const float length, const Vector3& target)
{
	m_graphicsEngine->SetShadowParam(direction, length, target);
}
