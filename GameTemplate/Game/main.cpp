#include "stdafx.h"
#include "system/system.h"
#include "stage_kari.h"
#include "LightManager.h"
#include "LevelObjectManager.h"


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

	NewGO<stage_kari>(0, "stage_kari");
	//ModelInitData modelInitData;
	//modelInitData.m_tkmFilePath = "Assets/modelData/test.tkm";
	//modelInitData.m_fxFilePath = "Assets/shader/myModel.fx";
	//Model model;
	//model.Init(modelInitData);
	//model.UpdateWorldMatrix(g_vec3Zero, g_quatIdentity, g_vec3One);
	//model.InIntersectLine({ 0, 100.0f, 0.0f }, { 0.0f, -50.0f, 0.0f });
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
		CLevelObjectManager::GetInstance()->ExecuteUpdate();
		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	CLightManager::DeleteInstance();
	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();
	CLevelObjectManager::DeleteInstance();
	return 0;
}

