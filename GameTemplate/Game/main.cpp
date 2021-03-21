#include "stdafx.h"
#include "system/system.h"
#include "Game.h"
#include "LightManager.h"
#include "LevelObjectManager.h"
#include "SoundEngine.h"


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
	
	CLightManager::CreateInstance();
	CLevelObjectManager::CreateInstance();
	CSoundEngine::CreateInstance();

	NewGO<Game>(0, "Game");

	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		//レンダリング開始。
		g_engine->BeginFrame();
		

		//////////////////////////////////////
		//ここから絵を描くコードを記述する。
		//////////////////////////////////////
		GameObjectManager::GetInstance()->ExecuteUpdate();
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		CLightManager::GetInstance()->ExecuteUpdate();
		CSoundEngine::GetInstance()->Update();
		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	CLightManager::DeleteInstance();
	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();
	CLevelObjectManager::DeleteInstance();
	CSoundEngine::DeleteInstance();


	return 0;
}

