#include "stdafx.h"
#include "StageBase.h"
#include "Title.h"

using namespace stageBaseConstData;

//スタート関数
bool IStageBase::Start()
{

	//ステージ開始時の演出の作成
	m_startDirecting = NewGO<CStartDirecting>(0, "StartDirecting");
	//タイトル画面か？
	if (m_isTitle) 
	{
		//タイトル画面
		//開始時の演出を行わない
		m_startDirecting->SetCheckStartDirecting(false);
	}
	else
	{
		//ゲーム画面
		//ゲーム画面では絶対ワイプインがある
		m_wipeInFlag = true;
	}

	//ワイプインフラグが立っていたら、ワイプインさせる
	//ワイプインフラグは外からtrueにされることもある。
	if (m_wipeInFlag)
		WipeIn();

	//ゲームカメラの作成
	m_gameCamea = NewGO<CGameCamera>(0, "GameCamera");
	//タイトル画面か？
	if (m_isTitle)
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
	if (m_isTitle)
	{
		//タイトル画面なら

		//キャパシティのタイトルフラグを立てる
		m_capacityUI->SetTitleFlag(true);
	}


	//Tipsコントローラーの生成
	m_tipsController = NewGO<CTipsController>(0);
	//タイトル画面か？
	if (m_isTitle)
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

	//プレイヤーのポインタ
	Player* pPlayer = nullptr;

	//ステージのメビウスの輪のポインタ
	CMobius* mobius = nullptr;



	//レベルをロードする
	m_level.Init(tklFilePath, [&](LevelObjectData& objData) {
		//ロードするレベル一つ一つにクエリを行う

		/*
		* 「基本オブジェクト」
		*/

		//オブジェクトネームがプレイヤーと同じだったら
		if (objData.EqualObjectName(LEVEL_OBJECT_NAME_PLAYER) == true)
		{
			//プレイヤーを作成する
			pPlayer = NewGO<Player>(PRIORITY_FIRST, GetGameObjectName(EN_GO_TYPE_PLAYER));
			//座標を設定する
			pPlayer->SetPosition(objData.position);
			//タイトル画面かどうかの確認
			//ゲーム画面の場合、座標が変更される
			pPlayer->SetTitleMove(m_isTitle);
			//フックしたため、trueを返す。
			return true;
		}
		//メビウスの輪
		else if (objData.EqualObjectName(LEVEL_OBJECT_NAME_MOBIUS) == true)
		{
			mobius = NewGO<CMobius>(PRIORITY_FIRST, GetGameObjectName(EN_GO_TYPE_MOBIUS));
			mobius->SetPosition(objData.position);
			mobius->SetRotation(objData.rotation);
			return true;
		}


		/*
		* 「反転オブジェクト」ReversibleObject
		*/

		////////////////////////////////////////////////////////////
		// 反転オブジェクトを作成時、ここに追加
		////////////////////////////////////////////////////////////

		//水、火
		//水
		else if (objData.EqualObjectName(GetLevelObjectName(EN_RO_TYPE_WATER)) == true)
		{
			//反転オブジェクトの生成
			NewReversibleObject<ROmizu_kori>(objData, EN_FRONT);
			return true;
		}
		//火
		else if (objData.EqualObjectName(GetLevelObjectName(EN_RO_TYPE_FIRE)) == true)
		{
			//反転オブジェクトの生成
			NewReversibleObject<ROmizu_kori>(objData, EN_BACK);
			return true;
		}
		//稼働、停止
		//稼働
		else if (objData.EqualObjectName(GetLevelObjectName(EN_RO_TYPE_RUNNING)) == true)
		{
			//反転オブジェクトの生成
			NewReversibleObject<ROrunning_stop>(objData, EN_FRONT);
			return true;
		}
		//停止
		else if (objData.EqualObjectName(GetLevelObjectName(EN_RO_TYPE_STOP)) == true)
		{
			//反転オブジェクトの生成
			NewReversibleObject<ROrunning_stop>(objData, EN_BACK);
			return true;
		}
		//釘、金槌
		//釘
		else if (objData.EqualObjectName(GetLevelObjectName(EN_RO_TYPE_NAIL)) == true)
		{
			//反転オブジェクトの生成
			NewReversibleObject<ROnail_bar>(objData, EN_FRONT);
			return true;
		}
		//金槌
		else if (objData.EqualObjectName(GetLevelObjectName(EN_RO_TYPE_HAMMER)) == true)
		{
			//反転オブジェクトの生成
			NewReversibleObject<ROnail_bar>(objData, EN_BACK);
			return true;
		}
		//鍵、南京錠
		//鍵
		else if (objData.EqualObjectName(GetLevelObjectName(EN_RO_TYPE_KEY)) == true)
		{
			//反転オブジェクトの生成
			NewReversibleObject<ROkey_padlock>(objData, EN_FRONT);
			return true;
		}
		//南京錠
		else if (objData.EqualObjectName(GetLevelObjectName(EN_RO_TYPE_PADLOCK)) == true)
		{
			//反転オブジェクトの生成
			NewReversibleObject<ROkey_padlock>(objData, EN_BACK);
			return true;
		}
		//左、右
		//左
		else if (objData.EqualObjectName(GetLevelObjectName(EN_RO_TYPE_LEFT)) == true)
		{
			//反転オブジェクトの生成
			NewReversibleObject<ROleft_right>(objData, EN_FRONT);
			return true;
		}
		//右
		else if (objData.EqualObjectName(GetLevelObjectName(EN_RO_TYPE_RIGHT)) == true)
		{
			//反転オブジェクトの生成
			NewReversibleObject<ROleft_right>(objData, EN_BACK);
			return true;
		}


		/*
		* 「障害オブジェクト」ObstacleObject
		*/

		////////////////////////////////////////////////////////////
		// 障害オブジェクトを作成時、ここに追加
		////////////////////////////////////////////////////////////

		//ゴール
		else if (objData.EqualObjectName(GetLevelObjectName(EN_OO_TYPE_GOAL)) == true)
		{
			//障害オブジェクトの生成
			//ゴールはメンバ変数で保持しておく。
			m_goal = NewObstacleObject<OOgoal>(objData);
			return true;
		}
		//炎
		else if (objData.EqualObjectName(GetLevelObjectName(EN_OO_TYPE_FLAME)) == true)
		{
			//障害オブジェクトの生成
			NewObstacleObject<OObigFire>(objData);
			return true;
		}
		//壁
		else if (objData.EqualObjectName(GetLevelObjectName(EN_OO_TYPE_WALL)) == true)
		{
			//障害オブジェクトの生成
			NewObstacleObject<OOwall>(objData);
			return true;
		}
		//でかい南京錠
		else if (objData.EqualObjectName(GetLevelObjectName(EN_OO_TYPE_BIG_PADLOCK)) == true)
		{
			//障害オブジェクトの生成
			NewObstacleObject<OOpadlock>(objData);
			return true;
		}
		//箱
		else if (objData.EqualObjectName(GetLevelObjectName(EN_OO_TYPE_BOX)) == true)
		{
			//障害オブジェクトの生成
			NewObstacleObject<OObox>(objData);
			return true;
		}
		//一方通行
		//左
		else if (objData.EqualObjectName(GetLevelObjectName(OO_TYPE_ONEWAY_LEFT)) == true)
		{
			//障害オブジェクトの生成
			OOoneway* OObject = NewObstacleObject<OOoneway>(objData);
			//左向きに設定
			OObject->SetLeftOrRight(OOoneway::enLeft);
			return true;
		}
		//右
		else if (objData.EqualObjectName(GetLevelObjectName(OO_TYPE_ONEWAY_RIGHT)) == true)
		{
			//障害オブジェクトの生成
			OOoneway* OObject = NewObstacleObject<OOoneway>(objData);
			//左向きに設定
			OObject->SetLeftOrRight(OOoneway::enRight);
			return true;
		}
		//全反転
		else if (objData.EqualObjectName(GetLevelObjectName(EN_OO_TYPE_REVERSE_ALL)) == true)
		{
			//障害オブジェクトの生成
			NewObstacleObject<OOReverseALL>(objData);
			return true;
		}
		//UFO
		else if (objData.EqualObjectName(GetLevelObjectName(EN_GO_TYPE_UFO)) == true)
		{
			//障害オブジェクトの生成
			//不透明モードで、オブジェクトの名前を付ける
			m_ufo = NewObstacleObject<CUFO>(
				objData,
				MODE_OPACITY,
				GetGameObjectName(EN_GO_TYPE_UFO)
				);
			return true;
		}
		//スイッチ
		else if (objData.EqualObjectName(GetLevelObjectName(EN_OO_TYPE_TRANSPARENT_SWITCH)) == true)
		{
			//障害オブジェクトの生成
			//不透明モードで、オブジェクトの名前を付けずに、優先度を二番目に設定する
			m_switch = NewObstacleObject<OOTransparentSwitch>(
				objData,
				MODE_OPACITY,
				NAME_EMPTY,
				PRIORITY_SECOND
				);
			//スイッチのタイマーを設定する
			m_switch->SetResetTimerStartValue(m_switchTime);
			return true;
		}



		////////////////////////////////////////////////////////////
		// 透明オブジェクトを作成時、ここに追加
		////////////////////////////////////////////////////////////

		/*
		* 透明反転オブジェクト
		*/

		//水、火（透明）
		//火（透明）
		else if (objData.EqualObjectName(GetLevelObjectName(RO_TYPE_FIRE_TRANSPARENT)) == true)
		{
			//反転オブジェクトの生成
			//透明モード
			NewReversibleObject<ROmizu_kori>(objData, EN_BACK, MODE_TRANS);
			return true;
		}
		//釘、金槌（透明）
		//釘（透明）
		else if (objData.EqualObjectName(GetLevelObjectName(RO_TYPE_NAIL_TRANSPARENT)) == true)
		{
			//反転オブジェクトの生成
			//透明モード
			NewReversibleObject<ROnail_bar>(objData, EN_FRONT, MODE_TRANS);
			return true;
		}
		//金槌（透明）
		else if (objData.EqualObjectName(GetLevelObjectName(RO_TYPE_HAMMER_TRANSPARENT)) == true)
		{
			//反転オブジェクトの生成
			//透明モード
			NewReversibleObject<ROnail_bar>(objData, EN_BACK, MODE_TRANS);
			return true;
		}
		//鍵、南京錠（透明）
		//鍵（透明）
		else if (objData.EqualObjectName(GetLevelObjectName(RO_TYPE_KEY_TRANSPARENT)) == true)
		{
			//反転オブジェクトの生成
			//透明モード
			NewReversibleObject<ROkey_padlock>(objData, EN_FRONT, MODE_TRANS);
			return true;
		}
		//南京錠（透明）
		else if (objData.EqualObjectName(GetLevelObjectName(RO_TYPE_PADLOCK_TRANSPARENT)) == true)
		{
			//反転オブジェクトの生成
			//透明モード
			NewReversibleObject<ROkey_padlock>(objData, EN_BACK, MODE_TRANS);
			return true;
		}

		/*
		* 透明障害オブジェクト
		*/

		//ゴール（透明）
		else if (objData.EqualObjectName(GetLevelObjectName(OO_TYPE_GOAL_TRANSPARENT)) == true)
		{
			//障害オブジェクトの生成
			//透明モード
			//ゴールはメンバ変数で保持しておく。
			m_goal = NewObstacleObject<OOgoal>(objData, MODE_TRANS);
			return true;
		}
		//炎（透明）
		else if (objData.EqualObjectName(GetLevelObjectName(OO_TYPE_FLAME_TRANSPARENT)) == true)
		{
			//障害オブジェクトの生成
			//透明モード
			NewObstacleObject<OObigFire>(objData, MODE_TRANS);
			return true;
		}
		//箱（透明）
		else if (objData.EqualObjectName(GetLevelObjectName(OO_TYPE_BOX_TRANSPARENT)) == true)
		{
			//障害オブジェクトの生成
			//透明モード
			NewObstacleObject<OObox>(objData, MODE_TRANS);
			return true;
		}


		/*
		* 「ウェイポイント」
		*/

		//オブジェクトネームに"waypoint"があったら
		else if (std::wcsstr(objData.name, OBJECT_NAME_WAYPOINT) != NULL)
		{
			//番号（"0"）の文字列があるアドレスを返す
			std::wstring buff = std::wcsstr(objData.name, CHAR_GET_WAYPOINT_NUM);
			//wstringをintに変換
			int num = _wtoi(buff.c_str());
			//マップに入れる
			posMap.insert(std::make_pair(num, objData.position));
			rotMap.insert(std::make_pair(num, objData.rotation));

			return true;
		}
		return false;
	});


	//ステージのメビウスの輪をマネージャーに登録
	CLevelObjectManager::GetInstance()->SetMobius(mobius);
	//プレイヤーをマネージャーに登録
	CLevelObjectManager::GetInstance()->SetPlayer(pPlayer);
	//ロードしたレベルにあったウェイポイントをマネージャーに登録する
	CLevelObjectManager::GetInstance()->InitWayPointPos(posMap);
	CLevelObjectManager::GetInstance()->InitWayPointRot(rotMap);

	//ウェイポイントをプレイヤーに設定する
	pPlayer->SetWayPointPos
	(CLevelObjectManager::GetInstance()->GetWayPointPos());
	pPlayer->SetWayPointRot
	(CLevelObjectManager::GetInstance()->GetWayPointRot());

	//ステージ開始時の演出時のカメラの注視点の座標を設定する
	m_startDirecting->SetPosition(pPlayer->GetPosition());




	return;
}

/**
 * @brief 反転オブジェクトを生成する関数
 * @note CLevelObjectManager::GetInstance()->AllDeleteLOs()でリソースは一気に開放できる
 * @tparam C 生成する反転オブジェクトの型
 * @param [in] objData ロードされたオブジェクトのデータ
 * @param [in] frontOrBack 表か裏か？
 * @param [in] Transparen 透明か？デフォルトはfalse
 * @param [in] objectName オブジェクトの名前。デフォルトはカラの名前
 * @param [in] priority ゲームオブジェクトの優先度。デフォルトは PRIORITY_FIRST
 * @return 生成した反転オブジェクトの参照
*/
template<class C>
C* IStageBase::NewReversibleObject(
	const LevelObjectData& objData,
	const bool frontOrBack,
	const bool transparent,
	const char* const objectName,
	const int priority
)
{
	//反転オブジェクトを生成
	C* RObject = NewGO<C>(priority, objectName);
	//座標を設定
	RObject->SetPosition(objData.position);
	//表か裏か？を設定
	RObject->SetFrontOrBack(frontOrBack);
	//透明のオブジェクトか？
	if (transparent == MODE_TRANS)
	{
		//透明オブジェクトに設定
		RObject->SetTransparentObject();
	}

	//生成した反転オブジェクトの参照を戻す
	return RObject;
}

/**
 * @brief 障害オブジェクトを生成する関数
 * @note CLevelObjectManager::GetInstance()->AllDeleteLOs()でリソースは一気に開放できる
 * @tparam C 生成する障害オブジェクトの型
 * @param [in] objData ロードされたオブジェクトのデータ
 * @param [in] Transparent 透明か？デフォルトはfalse
 * @param [in] objectName オブジェクトの名前。デフォルトはカラの名前
 * @param [in] priority ゲームオブジェクトの優先度。デフォルトは PRIORITY_FIRST
 * @return 生成した障害オブジェクトの参照
*/
template<class C>
C* IStageBase::NewObstacleObject(
	const LevelObjectData& objData,
	const bool transparent,
	const char* const objectName,
	const int priority
)
{
	//障害オブジェクトを生成
	C* OObject = NewGO<C>(priority, objectName);
	//座標を設定
	OObject->SetPosition(objData.position);
	//透明のオブジェクトか？
	if (transparent == MODE_TRANS)
	{
		//透明オブジェクトに設定
		OObject->SetTransparentObject();
	}

	//生成した障害オブジェクトの参照を戻す
	return OObject;
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
	QueryGOs<CMobius>("Mobius", [&](CMobius* mobius)->bool
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

	if (!m_isTitle) {
		BGMInteractive();
	}

	//ワイプインフラグが立っていたら
	if (m_wipeInFlag)
	{
		//ワイプが終了位置にいないか調べて、
		m_wipeInFlag = !g_graphicsEngine->GetSceneChange().IsWipeFinished();
		//終了位置まで来ていたら、ワイプの終了させる
		if (!m_wipeInFlag)
		{
			g_graphicsEngine->GetSceneChange().WipeEnd();
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
	g_graphicsEngine->GetSceneChange().WipeIn();
}

/// <summary>
/// ワイプアウトする
/// </summary>
/// <returns>ワイプアウトが終了したか？</returns>
bool IStageBase::WipeOut()
{
	//ワイプの状態が、開始位置なら
	if (g_graphicsEngine->GetSceneChange().GetWipeSize() == 0.0f)
		//ワイプアウトする
		g_graphicsEngine->GetSceneChange().RandomWipeOut();

	//ワイプが終わったかどうか？
	return g_graphicsEngine->GetSceneChange().IsWipeFinished();

}

/// <summary>
/// タイトルへ遷移する
/// </summary>
void IStageBase::GoTitle()
{
	//タイトルを生成
	CTitle* title = NewGO<CTitle>(0);
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
	if (m_isTitle) {
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