#include "stdafx.h"
#include "StageBase.h"
#include "Title.h"

//スタート関数
bool IStageBase::Start()
{

	//ステージ開始時の演出の作成
	m_startDirecting = NewGO<StartDirecting>(0, "StartDirecting");
	if (m_startUpStartDirecting == false) {
		m_startDirecting->SetCheckStartDirecting(false);
	}
	else
	{
		//開始演出があるなら、ワイプインフラグを立てる
		m_wipeInFlag = true;
	}

	//ワイプインフラグが立っていたら、ワイプインさせる
	//ワイプインフラグは外からtrueにされることもある。
	if (m_wipeInFlag)
		WipeIn();

	//ゲームカメラの作成
	m_gameCamea = NewGO<GameCamera>(0, "GameCamera");
	//タイトル画面か？
	if (m_titlePlayer)
	{
		//タイトル画面なら
		//タイトルモードにする
		m_gameCamea->TitleMode();
	}

	//ポーズ画面用クラスの作成
	m_pause = NewGO<CPause>(0, "Pause");

	//空を作る
	m_sky = NewGO<CSky>(0);
	m_sky->SetScale(1000.0f);


	//フォントレンダラーの生成
	m_goalSR = NewGO<CSpriteRender>(10);
	//初期化
	m_goalSR->Init
	("Assets/Image/Clear.DDS", 1024.0f, 512.0f, { 0.5f,0.5f }, AlphaBlendMode_Trans);
	m_goalSR->SetPosition({ 0.0f,250.0f,0.0f });
	m_goalSR->SetPostRenderFlag(true);

	//無効化する
	m_goalSR->Deactivate();


	//キャパシティを表示するスプライトの生成と初期化
	m_capacityUI = NewGO<CCapacityUI>(0);

	//タイトル画面か？
	if (m_titlePlayer)
	{
		//タイトル画面なら

		//キャパシティのタイトルフラグを立てる
		m_capacityUI->SetTitleFlag(true);
	}


	//Tipsコントローラーの生成
	m_tipsController = NewGO<CTipsController>(0);
	//タイトル画面か？
	if (m_titlePlayer)
	{
		//タイトル画面なら

		//Tipsをタイトルモードにする
		m_tipsController->SetTitleMode();
	}

	//m_goalSEのサウンドキューを生成する
	m_goalSE = NewGO<CSoundCue>(0);
	//m_goalSEのサウンドキューを、waveファイルを指定して初期化する。
	m_goalSE->Init(L"Assets/sound/goal.wav");
	//音量調節
	m_goalSE->SetVolume(0.5f);

	//セーブデータをデータメンバに代入
	m_highestClearStageNum = m_save.GetSaveData().highestClearStageNum;

	return StartSub();
}

/// <summary>
/// レベルのロード
/// </summary>
/// <param name="filePath">tklのファイルパス</param>
void IStageBase::LoadLevel(const char* tklFilePath)
{

	//ウェイポイントの「場所」を格納するマップ
	std::map<int, Vector3> posMap;
	//ウェイポイントの「回転」を格納するマップ
	std::map<int, Quaternion> rotMap;
	//ウェイポイントの数
	std::size_t vecSize = 0;

	//プレイヤーのポインタ
	Player* pPlayer = nullptr;

	//ステージのメビウスの輪のポインタ
	Mobius* mobius = nullptr;



	//レベルをロードする
	m_level.Init(tklFilePath, [&](LevelObjectData& objData)
		{//ロードするレベル一つ一つにクエリを行う

			///
			///
			/// 「基本オブジェクト」
			
			//オブジェクトネームが"player_kari"と同じだったら
			if (objData.EqualObjectName(L"player_kari") == true)
			{
				//プレイヤーを作成する
				pPlayer = NewGO<Player>(0, "Player");
				//プレイヤーのポジションをロードしたオブジェクトと同じポジションにする
				if (m_titlePlayer==false) {		//ゲーム画面の時
					pPlayer->SetPosition({ 0.0f,3000.0f,0.0f });	//	画面外に移動
				}
				else
				pPlayer->SetPosition(objData.position);
				//タイトル画面かどうかの確認
				pPlayer->SetTitleMove(m_titlePlayer);
				//フックしたため、trueを返す。
				return true;
			}
			else if (objData.EqualObjectName(L"Mobius") == true)
			{
				mobius = NewGO<Mobius>(0, "Mobius");
				mobius->SetPosition(objData.position);
				mobius->SetRotation(objData.rotation);
				return true;
			}
		

			///
			///
			/// 「反転オブジェクト」ReversibleObject
			 
			////////////////////////////////////////////////////////////
			// 反転オブジェクトを作成時、ここに追加
			////////////////////////////////////////////////////////////

			//mizu_kori
			else if (objData.EqualObjectName(L"mizu") == true)
			{
				ROmizu_kori* RObject;
				RObject = NewGO<ROmizu_kori>(0, "mizu_kori");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enFront);
				return true;
			}
			else if (objData.EqualObjectName(L"fire") == true)
			{
				ROmizu_kori* RObject;
				RObject = NewGO<ROmizu_kori>(0, "mizu_kori");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enBack);
				return true;
			}
			else if (objData.EqualObjectName(L"koori") == true)
			{
				ROmizu_kori* RObject;
				RObject = NewGO<ROmizu_kori>(0, "mizu_kori");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enBack);
				return true;
			}
			//mizu_kori_switch
			else if (objData.EqualObjectName(L"fire_switch") == true)
			{
				ROmizu_kori* RObject;
				RObject = NewGO<ROmizu_kori>(0, "mizu_kori");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enBack);
				RObject->SetTransparentObject();
				return true;
			}
			//bird_fish
			else if (objData.EqualObjectName(L"bird") == true)
			{
				RObird_fish* RObject;
				RObject = NewGO<RObird_fish>(0, "bird_fish");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enFront);
				return true;
			}
			else if (objData.EqualObjectName(L"fish") == true)
			{
				RObird_fish* RObject;
				RObject = NewGO<RObird_fish>(0, "bird_fish");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enBack);
				return true;
			}
			//running_stop
			else if (objData.EqualObjectName(L"kadou") == true)
			{
				ROrunning_stop* RObject;
				RObject = NewGO<ROrunning_stop>(0, "running_stop");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enFront);
				return true;
			}
			else if (objData.EqualObjectName(L"teishi") == true)
			{
				ROrunning_stop* RObject;
				RObject = NewGO<ROrunning_stop>(0, "running_stop");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enBack);
				return true;
			}
			//wire_string
			else if (objData.EqualObjectName(L"wire") == true)
			{
				ROwire_string* RObject;
				RObject = NewGO<ROwire_string>(0, "wire_string");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enFront);
				return true;
			}
			else if (objData.EqualObjectName(L"string") == true)
			{
				ROwire_string* RObject;
				RObject = NewGO<ROwire_string>(0, "wire_string");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enBack);
				return true;
			}
			//nail_bar
			else if (objData.EqualObjectName(L"nail") == true)
			{
				ROnail_bar* RObject;
				RObject = NewGO<ROnail_bar>(0, "nail_bar");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enFront);
				return true;
			}
			else if (objData.EqualObjectName(L"bar") == true)
			{
				ROnail_bar* RObject;
				RObject = NewGO<ROnail_bar>(0, "nail_bar");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enBack);
				return true;
			}
			//nail_bar_switch
			else if (objData.EqualObjectName(L"nail_switch") == true)
			{
			ROnail_bar* RObject;
			RObject = NewGO<ROnail_bar>(0, "nail_bar");
			RObject->SetPosition(objData.position);
			RObject->SetFrontOrBack(CReversibleObject::enFront);
			RObject->SetTransparentObject();
			//スイッチの処理
			return true;
			}
			else if (objData.EqualObjectName(L"bar_switch") == true)
			{
			ROnail_bar* RObject;
			RObject = NewGO<ROnail_bar>(0, "nail_bar");
			RObject->SetPosition(objData.position);
			RObject->SetFrontOrBack(CReversibleObject::enBack);
			RObject->SetTransparentObject();
			//スイッチの処理
			return true;
			}
			//axe_pickaxe
			else if (objData.EqualObjectName(L"axe") == true)
			{
				ROaxe_pickaxe* RObject;
				RObject = NewGO<ROaxe_pickaxe>(0, "axe_pickaxe");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enFront);
				return true;
			}
			else if (objData.EqualObjectName(L"pickaxe") == true)
			{
				ROaxe_pickaxe* RObject;
				RObject = NewGO<ROaxe_pickaxe>(0, "axe_pickaxe");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enBack);
				return true;
			}
			//keymold_empty
			else if (objData.EqualObjectName(L"key mold") == true)
			{
				ROkeymold_empty* RObject;
				RObject = NewGO<ROkeymold_empty>(0, "keymold_empty");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enFront);
				return true;
			}
			//key_padlock
			else if (objData.EqualObjectName(L"key") == true)
			{
			ROkey_padlock* RObject;
			RObject = NewGO<ROkey_padlock>(0, "key");
			RObject->SetPosition(objData.position);
			RObject->SetFrontOrBack(CReversibleObject::enFront);
			return true;
			}
			else if (objData.EqualObjectName(L"padlock") == true)
			{
			ROkey_padlock* RObject;
			RObject = NewGO<ROkey_padlock>(0, "padlock");
			RObject->SetPosition(objData.position);
			RObject->SetFrontOrBack(CReversibleObject::enBack);
			return true;
			}
			//keymold_padlock_switch
			else if (objData.EqualObjectName(L"key_switch") == true)
			{
			ROkey_padlock* RObject;
			RObject = NewGO<ROkey_padlock>(0, "key");
			RObject->SetPosition(objData.position);
			RObject->SetFrontOrBack(CReversibleObject::enFront);
			RObject->SetTransparentObject();
			//スイッチの処理
			return true;
			}
			else if (objData.EqualObjectName(L"padlock_switch") == true)
			{
			ROkey_padlock* RObject;
			RObject = NewGO<ROkey_padlock>(0, "padlock");
			RObject->SetPosition(objData.position);
			RObject->SetFrontOrBack(CReversibleObject::enBack);
			RObject->SetTransparentObject();
			//スイッチの処理
			return true;
			}
			//left_right
			else if (objData.EqualObjectName(L"left") == true)
			{
			ROleft_right* RObject;
			RObject = NewGO<ROleft_right>(0, "left_right");
			RObject->SetPosition(objData.position);
			RObject->SetFrontOrBack(CReversibleObject::enFront);
			return true;
			}
			else if (objData.EqualObjectName(L"right") == true)
			{
			ROleft_right* RObject;
			RObject = NewGO<ROleft_right>(0, "left_right");
			RObject->SetPosition(objData.position);
			RObject->SetFrontOrBack(CReversibleObject::enBack);
			return true;
			}

			///
			///
			/// 「障害オブジェクト」ObstacleObject
			
			////////////////////////////////////////////////////////////
			// 障害オブジェクトを作成時、ここに追加
			////////////////////////////////////////////////////////////

			//goal
			else if (objData.EqualObjectName(L"goal") == true)
			{
				//ゴールはメンバ変数で保持しておく。
				m_goal = NewGO<OOgoal>(0, "goal");
				m_goal->SetPosition(objData.position);
				return true;
			}
			//goal_switch
			else if (objData.EqualObjectName(L"goal_switch") == true)
			{
			//ゴールはメンバ変数で保持しておく。
			m_goal = NewGO<OOgoal>(0, "goal");
			m_goal->SetPosition(objData.position);
			m_goal->SetTransparentObject();
			return true;
			}
			//bigFire
			else if (objData.EqualObjectName(L"bigFire") == true)
			{
				OObigFire* OObject;
				OObject = NewGO<OObigFire>(0, "bigFire");
				OObject->SetPosition(objData.position);
				return true;
			}
			//bigFire_switch
			else if (objData.EqualObjectName(L"bigFire_switch") == true)
			{
			OObigFire* OObject;
			OObject = NewGO<OObigFire>(0, "bigFire");
			OObject->SetPosition(objData.position);
			OObject->SetTransparentObject();
			return true;
			}
			//wall
			else if (objData.EqualObjectName(L"wall") == true)
			{
				OOwall* OObject;
				OObject = NewGO<OOwall>(0, "wall");
				OObject->SetPosition(objData.position);
				return true;
			}
			//notHavePadlock
			else if (objData.EqualObjectName(L"notHavePadlock") == true)
			{
				OOpadlock* OObject;
				OObject = NewGO<OOpadlock>(0, "notHavePadlock");
				OObject->SetPosition(objData.position);
				return true;
			}
			//box
			else if (objData.EqualObjectName(L"box") == true)
			{
				OObox* OObject;
				OObject = NewGO<OObox>(0, "box");
				OObject->SetPosition(objData.position);
				return true;
			}
			//box_switch
			else if (objData.EqualObjectName(L"box_switch") == true)
			{
			OObox* OObject;
			OObject = NewGO<OObox>(0, "box");
			OObject->SetPosition(objData.position);
			OObject->SetTransparentObject();
			//スイッチの処理
			return true;
			}
			////////////////////////////////////////////////////////////
			// 透明オブジェクトを作成時、ここに追加
			////////////////////////////////////////////////////////////





			else if (objData.EqualObjectName(L"switch") == true)
			{
				m_switch = NewGO<OOTransparentSwitch>(1, "transparentSwitch");
				m_switch->SetPosition(objData.position);
				//スイッチのタイマーを設定する
				m_switch->SetResetTimerStartValue(m_switchTime);
				return true;
			}
			//oneway
			else if (objData.EqualObjectName(L"oneway_left") == true)
			{
			OOoneway* OObject;
			OObject = NewGO<OOoneway>(0, "oneway");
			OObject->SetPosition(objData.position);
			OObject->SetLeftOrRight(OOoneway::enLeft);
			return true;
			}
			else if (objData.EqualObjectName(L"oneway_right") == true)
			{
			OOoneway* OObject;
			OObject = NewGO<OOoneway>(0, "oneway");
			OObject->SetPosition(objData.position);
			OObject->SetLeftOrRight(OOoneway::enRight);
			return true;
			}
			//reverseAll
			else if (objData.EqualObjectName(L"reverseAll") == true)
			{
			OOReverseALL* OObject;
			OObject = NewGO<OOReverseALL>(0, "oneway");
			OObject->SetPosition(objData.position);
			return true;
			}
			////switch
			//else if (objData.EqualObjectName(L"switch") == true)
			//{
			//RObird_fish* RObject;
			//RObject = NewGO<RObird_fish>(0, "bird_fish");
			//RObject->SetPosition(objData.position);
			//RObject->SetFrontOrBack(CReversibleObject::enFront);
			//return true;
			//}

			///
			///
			/// 「UFO」

			//UFO
			else if (objData.EqualObjectName(L"UFO") == true)
			{
			m_ufo = NewGO<CUFO>(0, "UFO");
			m_ufo->SetPosition(objData.position);
			return true;
			}


			///
			///
			/// 「ウェイポイント」

			//オブジェクトネームに"waypoint"があったら
			else if (std::wcsstr(objData.name, L"waypoint") != NULL)
			{
				//番号（"0"）の文字列があるアドレスを返す
				std::wstring buff = std::wcsstr(objData.name, L"0");
				//wstringをintに変換
				int num = _wtoi(buff.c_str());
				//マップに入れる
				posMap.insert(std::make_pair(num, objData.position));
				rotMap.insert(std::make_pair(num, objData.rotation));
				//ウェイポイントの数を加算
				vecSize++;


				//デバック用
				//後で消す
				//CModelRender* dbgModel;
				//dbgModel = NewGO<CModelRender>(0, "waypoint");
				//dbgModel->Init("Assets/modelData/yuka.tkm");
				//dbgModel->SetPosition(objData.position);
				//dbgModel->SetRotation(objData.rotation);
				//デバック用ここまで

				return true;
			}
			return false;
		});


	//ステージのメビウスの輪をマネージャーに登録
	CLevelObjectManager::GetInstance()->SetMobius(mobius);
	//プレイヤーをマネージャーに登録
	CLevelObjectManager::GetInstance()->SetPlayer(pPlayer);
	//ロードしたレベルにあったウェイポイントをマネージャーに登録する
	CLevelObjectManager::GetInstance()->InitWayPointPos(vecSize, posMap);
	CLevelObjectManager::GetInstance()->InitWayPointRot(vecSize, rotMap);

	//ウェイポイントをプレイヤーに設定する
	pPlayer->SetWayPointPos
	(vecSize, CLevelObjectManager::GetInstance()->GetWayPointPos());
	pPlayer->SetWayPointRot
	(vecSize, CLevelObjectManager::GetInstance()->GetWayPointRot());
	pPlayer->SetWayPointOBB();

	//ステージ開始時の演出時のカメラの注視点の座標を設定する
	m_startDirecting->SetPosition(pPlayer->GetPosition());
	//ウェイポイントをステージ開始時の演出に設定する
	/*m_startDirecting->SetWayPointPos
	(vecSize, CLevelObjectManager::GetInstance()->GetWayPointPos());
	m_startDirecting->SetWayPointRot
	(vecSize, CLevelObjectManager::GetInstance()->GetWayPointRot());*/



	return;
}

//デストラクタ
IStageBase::~IStageBase()
{
	//単体のオブジェクトを消去
	DeleteGO(m_gameCamea);
	DeleteGO(m_pause);
	DeleteGO(m_sky);
	DeleteGO(m_startDirecting);
	DeleteGO(m_goalSR);

	DeleteGO(m_bgmStage1);
	DeleteGO(m_bgmStage2);
	DeleteGO(m_loop_bgmStage1);
	DeleteGO(m_loop_bgmStage2);

	DeleteGO(m_capacityUI);


	DeleteGO(m_tipsController);

	//レベルでロードしたオブジェクトを消去

	////////////////////////////////////////////////////////////
	// 新しいオブジェクトを作成時、ここでDeleteGOすること
	////////////////////////////////////////////////////////////

	//「基本オブジェクト」
	QueryGOs<Player>("Player", [&](Player* player)->bool
		{
			DeleteGO(player);
			return true;
		}
	);
	QueryGOs<Mobius>("Mobius", [&](Mobius* mobius)->bool
		{
			DeleteGO(mobius);
			return true;
		}
	);


	//レベルオブジェクト
	//（ReversibleObject、ObstacleObjectのこと）
	//を全部消去
	CLevelObjectManager::GetInstance()->AllDeleteLOs();


	//デバック用
	//後で消す
	//QueryGOs<CModelRender>("waypoint", [&](CModelRender* waypoint)->bool
	//	{
	//		DeleteGO(waypoint);
	//		return true;
	//	}
	//);
	//デバック用ここまで
}


//アップデート関数
void IStageBase::Update()
{
	CheckGoal();

	//Start関数ではなくUpdate関数でBGMを初期化しているのは
	//ステージをロード中にBGMが再生しないようにするため。

	//BGMを初期化していなかったら
	if (!m_initedBGM)
	{
		//BGMを初期化する
		InitBGM();
	}

	if (m_startBGM == true) {
		BGMInteractive();
	}

	//ワイプインフラグが立っていたら
	if (m_wipeInFlag)
	{
		//ワイプが終了位置にいないか調べて、
		m_wipeInFlag = !g_sceneChange->IsWipeFinished();
		//終了位置まで来ていたら、ワイプの終了させる
		if (!m_wipeInFlag)
		{
			g_sceneChange->WipeEnd();
			m_gameCamea->SetWipeEndFlag(true);
		}
	}

	return;
}

//ポーズ中のみ呼ばれるアップデート関数
void IStageBase::UpdateOnlyPaused()
{
	//ポーズの状態を調べる

	if (m_pause->GetRetryFlag())
	{
		//リトライ
		Retry();
	}
	else if (m_pause->GetQuitFlag())
	{
		//終了
		Quit();
	}
}

/// <summary>
/// クリアした時の処理
/// </summary>
void IStageBase::Clear()
{
	if (!WipeOut())
		return;

	//セーブデータの構造体
	SSaveData saveData;
	//今クリアしたステージの番号が
	//セーブされている一番高いクリアしたステージの番号より大きいか？
	if (m_save.GetSaveData().highestClearStageNum < m_stageNum)
	{
		//大きかったら

		//セーブデータに今クリアしたステージの番号を代入する
		saveData.highestClearStageNum = m_stageNum;
		//セーブデータクラスにセーブデータを登録する
		m_save.SetSaveData(saveData);
	}
	//セーブをする
	m_save.SaveData();

	//タイトルの戻る
	GoTitle();
	Release();	//リリース
}

/// <summary>
/// リトライした時の処理
/// </summary>
void IStageBase::Retry()
{
	if (!WipeOut())
		return;
	//オーバーライドされているはずの処理を行う
	RetryStage();
	Release();	//リリース
}

/// <summary>
///	終了した時の処理
/// </summary>
void IStageBase::Quit()
{
	if (!WipeOut())
		return;
	//タイトルの戻る
	GoTitle();
	Release();	//リリース
}

/// <summary>
/// ゴールした時の処理
/// </summary>
void IStageBase::Goal()
{
	if (m_goalSEcount < 1) {				//一度だけ呼ぶ
		m_goalSE->Play(false);			//m_goalSEをループ再生をオフで再生する。
		m_goalSEcount++;
		m_goalSR->Activate();

	}
	
	m_player->GetPosition();
	//ゴールした後の時間を計測する	//デルタタイムを掛ける
	m_goalTimer += GameTime().GetFrameDeltaTime();

	//一定時間たったら
	if (m_goalTimer >= 4.0f)
	{
		//クリアする
		Clear();
	}
}

void IStageBase::CheckGoal()
{

	//アニメーションの初期化でエラーが出るのを調べる用のコード
	//デバック用
	//後で消す
	//ゴールしていたら、ゴールの処理をする
	//Goal();
	//return;
	//デバック用ここまで

	//ゴールがアクティブなら
	if (m_goal)
	{
		//ゴール状態を調べる
		if (m_goal->GetIsGoal())
		{
			//ゴールしていたら、ゴールの処理をする
			Goal();

			//ゴールしたらポーズができないようにする
			m_pause->SetCanPause(false);

			//UFOがあったら
			if (m_ufo)
				//動かなくして、プレイヤーを探さないようにする
				m_ufo->SetMoveSpeed(0.0f);
			//スイッチがあったら
			if (m_switch)
				//タイマーがカウントしなくなるようにする
				m_switch->SetTimerCountFlag(false);
		}
	}

	//Quit();
}

/// <summary>
/// ワイプインする
/// </summary>
void IStageBase::WipeIn()
{
	//ワイプインする
	g_sceneChange->WipeIn();
}

/// <summary>
/// ワイプアウトする
/// </summary>
/// <returns>ワイプアウトが終了したか？</returns>
bool IStageBase::WipeOut()
{
	//ワイプの状態が、開始位置なら
	if (g_sceneChange->GetWipeSize() == 0.0f)
		//ワイプアウトする
		g_sceneChange->RandomWipeOut();

	//ワイプが終わったかどうか？
	return g_sceneChange->IsWipeFinished();

}

/// <summary>
/// タイトルへ遷移する
/// </summary>
void IStageBase::GoTitle()
{
	//タイトルを生成
	Title* title = NewGO<Title>(0);
	//ワイプインするように設定する
	title->SetWipeInFlag(true);
}

/// <summary>
/// BGMの初期化
/// </summary>
void IStageBase::InitBGM()
{
	//////////
	//ステージBGMの初期設定
	//////////

	//BGMのサウンドキューを生成する
	m_bgmStage1 = NewGO<CSoundCue>(0);
	//BGMのサウンドキューを、waveファイルを指定して初期化する。
	m_bgmStage1->Init(L"Assets/sound/Stage1.wav");
	m_bgmStage1->SetSoundType(CSoundCue::enBGM);
	//BGMをループ再生をオンで再生する。
	m_bgmStage1->Play(true);
	//もしタイトル画面だった場合、BGM音量を０にする。
	if (m_startBGM == false) {
		m_bgmStage1->SetVolume(0.0f);
	}

	//BGMのサウンドキューを生成する
	m_bgmStage2 = NewGO<CSoundCue>(0);
	//BGMのサウンドキューを、waveファイルを指定して初期化する。
	m_bgmStage2->Init(L"Assets/sound/Stage2.wav");
	m_bgmStage2->SetSoundType(CSoundCue::enBGM);
	//BGMをループ再生をオンで再生する。
	m_bgmStage2->Play(true);
	//BGM音量を０にする。
	m_bgmStage2->SetVolume(0.0f);


	//BGMのサウンドキューを生成する
	m_loop_bgmStage1 = NewGO<CSoundCue>(0);
	//BGMのサウンドキューを、waveファイルを指定して初期化する。
	m_loop_bgmStage1->Init(L"Assets/sound/Stage1_Loop.wav");
	m_loop_bgmStage1->SetSoundType(CSoundCue::enBGM);
	//BGM音量を０にする。
	m_loop_bgmStage1->SetVolume(0.0f);

	//BGMのサウンドキューを生成する
	m_loop_bgmStage2 = NewGO<CSoundCue>(0);
	//BGMのサウンドキューを、waveファイルを指定して初期化する。
	m_loop_bgmStage2->Init(L"Assets/sound/Stage2_Loop.wav");
	m_loop_bgmStage2->SetSoundType(CSoundCue::enBGM);
	//BGM音量を０にする。
	m_loop_bgmStage2->SetVolume(0.0f);

	m_initedBGM = true;
}

void IStageBase::BGMInteractive()
{
	if (!m_player)
	{
		m_player = FindGO<Player>("Player");
		if (!m_player)
			return;
	}

	//ウェイポイントが24～7の場合
	if (24 <= m_player->GetWayPointState() && m_player->GetWayPointState() <= 31 ||
		0 <= m_player->GetWayPointState() && m_player->GetWayPointState() <= 7) {
		if (m_bgmStage1->GetVolume() < 1.0f) {
			m_bgmStage1->SetVolume(m_bgmStage1->GetVolume() + 0.01f);
			m_bgmStage2->SetVolume(m_bgmStage2->GetVolume() - 0.01f);
		}
	}

	//ウェイポイントが8～23の場合
	if (8 <= m_player->GetWayPointState() && m_player->GetWayPointState() <= 23) {
		if (m_bgmStage2->GetVolume() < 1.0f) {
			m_bgmStage2->SetVolume(m_bgmStage2->GetVolume() + 0.01f);
			m_bgmStage1->SetVolume(m_bgmStage1->GetVolume() - 0.01f);
		}
	}



	//ループ処理
	if (m_check_loop1 == false && m_bgmStage1->IsLoop()) {
		m_bgmStage1->Stop();
		m_loop_bgmStage1->Play(true);
		m_check_loop1 = true;
	}
	if (m_check_loop2 == false && m_bgmStage2->IsLoop()) {
		m_bgmStage2->Stop();
		m_loop_bgmStage2->Play(true);
		m_check_loop2 = true;
	}


	//ループの方のBGM音量をセット
	if (m_check_loop1 == true) {
		m_loop_bgmStage1->SetVolume(m_bgmStage1->GetVolume());
	}
	if (m_check_loop2 == true) {
		m_loop_bgmStage2->SetVolume(m_bgmStage2->GetVolume());
	}
}