#include "stdafx.h"
#include "system/system.h"
#include "Game.h"
#include "LightManager.h"
#include "LevelObjectManager.h"
#include "SoundEngine.h"
#include "OBBWorld.h"


///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//////////////////////////////////////
	// ここから初期化を行うコードを記述する。
	//////////////////////////////////////

	//ゲームオブジェクトマネージャーのインスタンスを作成する。
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();
	
	//各管理クラスのインスタンスを生成する。
	CLightManager::CreateInstance();		//ライトマネージャー
	CLevelObjectManager::CreateInstance();	//レベルオブジェクトマネージャー
	CSoundEngine::CreateInstance();			//サウンドエンジン
	COBBWorld::CreateInstance();			//OBBワールド

	//ゲームのインスタンスを生成する
	Game* game = NewGO<Game>(0, "Game");

	//シャドウ生成
	g_engine->CreateShadow({ 1.0f,-1.0f,-1.0f }, 1000.0f);
	//ディレクションライトの生成
	CDirectionLight* gameDirectionLight = nullptr;
	gameDirectionLight = NewGO<CDirectionLight>(0, "GameDirectionLight");
	gameDirectionLight->SetDirection({ 1.0f,-1.0f,-1.0f });
	gameDirectionLight->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////

	auto& renderContext = g_graphicsEngine->GetRenderContext();	//レンダーコンテキスト

	//メインレンダーターゲット
	RenderTarget& mainRenderTarget = g_graphicsEngine->GetMainRenderTarget();


	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		//レンダリング開始。
		g_engine->BeginFrame();

		//////////////////////////////////////////////////////////////
		///	アップデートの処理開始
		//////////////////////////////////////////////////////////////

		//ゲームオブジェクトのアップデート
		GameObjectManager::GetInstance()->ExecuteUpdate();
		//ライトのアップデート
		CLightManager::GetInstance()->ExecuteUpdate();
		//サウンドエンジンのアップデート
		CSoundEngine::GetInstance()->Update();

		//////////////////////////////////////////////////////////////
		///	アップデートの処理終了
		//////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////
		///	レンダリングの処理開始
		//////////////////////////////////////////////////////////////

		//シャドウイング
		g_engine->DrawShadow();

		//メインレンダリングターゲットをセットする
		g_engine->UseMainRenderTarget();

		//メインのレンダリングを実行
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);

		//メインレンダリングターゲットの書き込み終了待ち
		g_engine->WaitDrawingMainRenderTarget();

		//ポストエフェクト
		g_engine->DrawPostEffect();

		//場面転換の2D演出
		g_engine->DrawSceneChange();

		//メインレンダリングターゲットの絵をフレームバッファーに描画する
		g_engine->DrawFrameBuffer();

		//////////////////////////////////////////////////////////////
		///	レンダリングの処理終了
		//////////////////////////////////////////////////////////////

		//レンダリング終了
		g_engine->EndFrame();
	}

	//ディレクションライトを破棄
	DeleteGO(gameDirectionLight);

	//ゲームのインスタンスを破棄
	DeleteGO(game);


	//各管理クラスのインスタンスを破棄する。
	COBBWorld::DeleteInstance();			//OBBワールド
	CSoundEngine::DeleteInstance();			//サウンドエンジン
	CLevelObjectManager::DeleteInstance();	//レベルオブジェクトマネージャー
	CLightManager::DeleteInstance();		//ライトマネージャー

	//ゲームオブジェクトマネージャーを削除。
	PhysicsWorld::DeleteInstance();
	GameObjectManager::DeleteInstance();


	return 0;
}

