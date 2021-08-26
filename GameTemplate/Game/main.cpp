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
#include "TitleNameData.h"

//メビリンスを使用可能にする
using namespace nsMobyrinth;

/**
 * @brief 必要なインスタンス達を生成する
*/
void CreateInstances();

/**
 * @brief 生成したインスタンスたちを破棄する
*/
void DeleteInstances();

/**
 * @brief ゲーム全体を通して使うディレクションライトを生成する
 * @param [out] directionLight ディレクションライトのポインタ
*/
void CreateGameDirectionLigth(nsLight::CDirectionLight* directionLight);

/**
 * @brief アップデート処理
*/
void Updating();

/**
 * @brief レンダリング処理
*/
void Rendering(RenderContext& renderContext);


///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, titleNameData::TITLE_NAME);

	//////////////////////////////////////
	// ここから初期化を行うコードを記述する。
	//////////////////////////////////////

	//必要なインスタンス達を生成する
	CreateInstances();

	//ストップウォッチを生成する
	nsTimer::CStopWatch stopWatch;

	//ゲームのインスタンスを生成する
	CGame* game = NewGO<CGame>(nsCommonData::PRIORITY_FIRST,
		nsCommonData::GetGameObjectName(nsCommonData::EN_GO_TYPE_GAME));

	//ゲーム全体を通して使うディレクションライト
	nsLight::CDirectionLight* gameDirectionLight = nullptr;
	//ゲーム全体を通して使うディレクションライトを生成する
	CreateGameDirectionLigth(gameDirectionLight);


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

		//アップデート処理
		Updating();

		//////////////////////////////////////////////////////////////
		///	アップデートの処理終了
		//////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////
		///	レンダリングの処理開始
		//////////////////////////////////////////////////////////////

		//レンダリング処理
		Rendering(renderContext);

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
			restTime = nsTimer::STOP_WATCH_CONST_DATA::MILLISECOND_FOR_LOCK_60FPS -
				static_cast<int>(stopWatch.GetElapsedMillisecond());
		} while (restTime > 0);

		//ストップウォッチの計測終了
		stopWatch.Stop();

		//デルタタイムをストップウォッチの計測時間から、計算する
		nsTimer::GameTime().PushFrameDeltaTime((float)stopWatch.GetElapsed());
	}

	//ディレクションライトを破棄
	DeleteGO(gameDirectionLight);

	//ゲームのインスタンスを破棄
	DeleteGO(game);

	//生成したインスタンスたちを破棄する
	DeleteInstances();

	return 0;
}


/**
 * @brief 必要なインスタンス達を生成する
*/
void CreateInstances()
{
	//ゲームオブジェクトマネージャーのインスタンスを作成する。
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();

	//各管理クラスのインスタンスを生成する。
	nsLight::CLightManager::CreateInstance();		//ライトマネージャー
	nsLevelObject::CLevelObjectManager::CreateInstance();	//レベルオブジェクトマネージャー
	nsSound::CSoundEngine::CreateInstance();			//サウンドエンジン
	nsOBB::COBBWorld::CreateInstance();			//OBBワールド

	//エフェクトエンジンのインスタンスを作成する。
	EffectEngine::CreateInstance();


	//シャドウの生成と初期化
	g_graphicsEngine->GetShadowMap().CreateShadowMap(
		nsGraphic::nsShadow::shadowConstData::SHADOW_INIT_DIRECTION,
		nsGraphic::nsShadow::shadowConstData::SHADOW_INIT_LENGTH
	);

	return;
}

/**
 * @brief 生成したインスタンスたちを破棄する
*/
void DeleteInstances()
{
	//エフェクトエンジンのインスタンスの破棄
	EffectEngine::DeleteInstance();

	//各管理クラスのインスタンスを破棄する。
	nsOBB::COBBWorld::DeleteInstance();			//OBBワールド
	nsSound::CSoundEngine::DeleteInstance();			//サウンドエンジン
	nsLevelObject::CLevelObjectManager::DeleteInstance();	//レベルオブジェクトマネージャー
	nsLight::CLightManager::DeleteInstance();		//ライトマネージャー

	//ゲームオブジェクトマネージャーを削除。
	PhysicsWorld::DeleteInstance();
	GameObjectManager::DeleteInstance();

	return;
}


/**
 * @brief ゲーム全体を通して使うディレクションライトを生成する
 * @param [out] directionLight ディレクションライトのポインタ
*/
void CreateGameDirectionLigth(nsLight::CDirectionLight* directionLight)
{
	directionLight = NewGO<nsLight::CDirectionLight>(
		nsCommonData::PRIORITY_FIRST,
		nsCommonData::GetGameObjectName(nsCommonData::EN_GO_TYPE_GAME_DIRECTION_LIGHT)
		);
	directionLight->SetDirection(
		nsLight::directionLightConstData::DIRECTIONLIG_INIT_DIRECTION
	);
	directionLight->SetColor(nsLight::directionLightConstData::DIRECTIONLIG_INIT_COLOR);

	return;
}

/**
 * @brief アップデート処理
*/
void Updating()
{
	//ゲームオブジェクトのアップデート
	GameObjectManager::GetInstance()->ExecuteUpdate();
	//エフェクトエンジンのアップデート。
	EffectEngine::GetInstance()->Update(nsTimer::GameTime().GetFrameDeltaTime());
	//ライトのアップデート
	nsLight::CLightManager::GetInstance()->Update();
	//サウンドエンジンのアップデート
	nsSound::CSoundEngine::GetInstance()->Update();

	return;
}

/**
 * @brief レンダリング処理
*/
void Rendering(RenderContext& renderContext)
{
	//シャドウイング
	g_graphicsEngine->GetShadowMap().Draw(renderContext);

	// ZPrepass
	g_graphicsEngine->ZPrepass(renderContext);

	//ディファ―ドレンダリングを使用可能にする
	g_graphicsEngine->UseDefferdRendering();

	//G-Bufferの作成を実行
	GameObjectManager::GetInstance()->ExecuteRender(renderContext);

	//G-Bufferの作成待ち
	g_graphicsEngine->WaitDefferdRenderring();

	//メインレンダリングターゲットをセットする
	g_graphicsEngine->UseMainRenderTarget();

	//G-Bufferを使用してディファ―ドライティング用のスプライトの描画処理
	g_graphicsEngine->DrawDefferdRenderSprite();

	renderContext.SetRenderTarget(
		g_graphicsEngine->GetMainRenderTarget().GetRTVCpuDescriptorHandle(),
		g_graphicsEngine->GetGBufferDSVCpuDescriptorHandle()
	);

	//メインのレンダリングを実行
	GameObjectManager::GetInstance()->ExecuteForwardRender(renderContext);

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

	GameObjectManager::GetInstance()->ExecuteFontRender(renderContext);

	//HUD
	g_graphicsEngine->HUDRender();

	//メインレンダリングターゲットの絵をフレームバッファーに描画する
	g_graphicsEngine->CopyToFrameBuffer();

	//登録されている3Dモデルをクリア
	g_graphicsEngine->ClearZPrepassModels();

	return;
}