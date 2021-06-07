#include "stdafx.h"
#include "system/system.h"
#include "Game.h"
#include "LightManager.h"
#include "LevelObjectManager.h"
#include "SoundEngine.h"
#include "OBBWorld.h"
#include "StopWatch.h"
#include "GameTime.h"
#include "effect/effect.h"


///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("メビリンス"));

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

	//エフェクトエンジンのインスタンスを作成する。
	EffectEngine::CreateInstance();

	//ストップウォッチを生成する
	CStopWatch stopWatch;

	//ゲームのインスタンスを生成する
	Game* game = NewGO<Game>(PRIORITY_FIRST, "Game");

	//シャドウの生成と初期化
	g_graphicsEngine->GetShadowMap().CreateShadowMap(
		shadowConstData::SHADOW_INIT_DIRECTION,
		shadowConstData::SHADOW_INIT_LENGTH
	);

	//ディレクションライトの生成と初期化
	CDirectionLight* gameDirectionLight = nullptr;
	gameDirectionLight = NewGO<CDirectionLight>(PRIORITY_FIRST, "GameDirectionLight");
	gameDirectionLight->SetDirection(directionLightConstData::DIRECTIONLIG_INIT_DIRECTION);
	gameDirectionLight->SetColor(directionLightConstData::DIRECTIONLIG_INIT_COLOR);

	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////

	auto& renderContext = g_graphicsEngine->GetRenderContext();	//レンダーコンテキスト

	//メインレンダーターゲット
	RenderTarget& mainRenderTarget = g_graphicsEngine->GetMainRenderTarget();


	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		//ストップウォッチの計測開始
		stopWatch.Start();

		//レンダリング開始。
		g_engine->BeginFrame();

		//////////////////////////////////////////////////////////////
		///	アップデートの処理開始
		//////////////////////////////////////////////////////////////

		//ゲームオブジェクトのアップデート
		GameObjectManager::GetInstance()->ExecuteUpdate();
		//エフェクトエンジンのアップデート。
		EffectEngine::GetInstance()->Update(GameTime().GetFrameDeltaTime());
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
		g_graphicsEngine->GetShadowMap().Draw(renderContext);

		// ZPrepass
		g_graphicsEngine->ZPrepass(renderContext);

		//メインレンダリングターゲットをセットする
		g_graphicsEngine->UseMainRenderTarget();

		//メインのレンダリングを実行
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);

		//エフェクトのドロー。
		EffectEngine::GetInstance()->Draw();

#ifdef MY_DEBUG
		//FPSを描画する
		GameTime().DrawFPS(renderContext, (float)stopWatch.GetElapsed());
#endif

		//メインレンダリングターゲットの書き込み終了待ち
		g_graphicsEngine->WaitDrawingMainRenderTarget();

		//ポストエフェクト
		g_graphicsEngine->DrawPostEffect(renderContext);

		//場面転換の2D演出
		g_graphicsEngine->SceneChangeRender();

		//メインレンダリングターゲットの絵をフレームバッファーに描画する
		g_graphicsEngine->CopyToFrameBuffer();

		//登録されている3Dモデルをクリア
		g_graphicsEngine->ClearZPrepassModels();

		//////////////////////////////////////////////////////////////
		///	レンダリングの処理終了
		//////////////////////////////////////////////////////////////

		//レンダリング終了
		g_engine->EndFrame();


		//FPS固定の処理
		//スピンロックを行う。
		int restTime = 0;
		do {
			stopWatch.Stop();
			restTime = 16 - (int)stopWatch.GetElapsedMillisecond();
		} while (restTime > 0);

		//ストップウォッチの計測終了
		stopWatch.Stop();

		//デルタタイムをストップウォッチの計測時間から、計算する
		GameTime().PushFrameDeltaTime((float)stopWatch.GetElapsed());
	}

	//ディレクションライトを破棄
	DeleteGO(gameDirectionLight);

	//ゲームのインスタンスを破棄
	DeleteGO(game);

	//エフェクトエンジンのインスタンスの破棄
	EffectEngine::DeleteInstance();

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

