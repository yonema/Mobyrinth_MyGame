#include "stdafx.h"
#include "StageBase.h"
#include "Title.h"

/**
 * @brief メビリンス
*/
namespace nsMobyrinth
{
	/**
	 * @brief ステージ
	*/
	namespace nsStage
	{
		//ステージベースの定数データを使用可能にする
		using namespace stageBaseConstData;
		//レベルオブジェクトの定数データを使用可能にする
		using namespace nsLevelObject;
		//反転オブジェクトの定数データを使用可能にする
		using namespace nsLevelObject::nsReversibleObject;
		//障害オブジェクトの定数データを使用可能にする
		using namespace nsLevelObject::nsObstacleObject;
		//共通定数データを使用可能にする
		using namespace nsCommonData;

		//スタート関数
		bool IStageBase::Start()
		{
			//この関数限定で
			//タイトルを使用可能にする
			using namespace nsTitle;

			//ゲームカメラの作成
			m_gameCamea = NewGO<nsCamera::CGameCamera>(PRIORITY_FIRST, GetGameObjectName(EN_GO_TYPE_GAME_CAMERA));


			//ポーズ画面用クラスの作成
			m_pause = NewGO<nsUI::CPause>(PRIORITY_FIRST, GetGameObjectName(EN_GO_TYPE_PAUSE));

			//空を作る
			m_sky = NewGO<nsSky::CSky>(PRIORITY_FIRST);
			m_sky->SetScale(MODEL_SCALE_SKY);
			switch (m_stageNum)
			{
			case titleConstData::EN_STAGE_1:
			case titleConstData::EN_STAGE_2:
			case titleConstData::EN_STAGE_3:
				m_sky->SetSkyCubeTextureFilepath(nsSky::skyConstData::TEXTURE_FILEPATH_SKY_DEFAULT);
				break;
			case titleConstData::EN_STAGE_4:
			case titleConstData::EN_STAGE_5:
			case titleConstData::EN_STAGE_6:
				m_sky->SetSkyCubeTextureFilepath(nsSky::skyConstData::TEXTURE_FILEPATH_SKY_EVENING);
				break;
			case titleConstData::EN_STAGE_7:
			case titleConstData::EN_STAGE_8:
			case titleConstData::EN_STAGE_9:
				m_sky->SetSkyCubeTextureFilepath(nsSky::skyConstData::TEXTURE_FILEPATH_SKY_NIGHT);
				break;
			}


			//キャパシティを表示するスプライトの生成と初期化
			m_capacityUI = NewGO<nsUI::CCapacityUI>(PRIORITY_FIRST);


			//Tipsコントローラーの生成
			m_tipsController = NewGO<nsTips::CTipsController>(PRIORITY_FIRST);


			//セーブデータをデータメンバに保持させる
			m_highestClearStageNum = m_save.GetSaveData().highestClearStageNum;


			//タイトル画面か？
			if (m_isTitle)
			{
				//タイトル画面なら

				//ゲームカメラをタイトルモードにする
				m_gameCamea->TitleMode();
				//キャパシティのタイトルフラグを立てる
				m_capacityUI->SetTitleFlag(true);
				//Tipsをタイトルモードにする
				m_tipsController->SetTitleMode();
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
			nsMobyrinth::nsPlayer::Player* pPlayer = nullptr;

			//ステージのメビウスの輪のポインタ
			nsMobius::CMobius* mobius = nullptr;

			//ウェイポイントを見るためのコード
			//static bool InitedWayPoint = false;


			//レベルをロードする
			m_level.Init(tklFilePath, [&](nsTkLevel::LevelObjectData& objData) {
				//ロードするレベル一つ一つにクエリを行う

				/*
				* 「基本オブジェクト」
				*/

				//オブジェクトネームがプレイヤーと同じだったら
				if (objData.EqualObjectName(LEVEL_OBJECT_NAME_PLAYER) == true)
				{
					//プレイヤーを作成する
					pPlayer = NewGO<nsMobyrinth::nsPlayer::Player>(PRIORITY_FIRST, GetGameObjectName(EN_GO_TYPE_PLAYER));
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
					mobius = NewGO<nsMobius::CMobius>(PRIORITY_FIRST, GetGameObjectName(EN_GO_TYPE_MOBIUS));
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
					NewReversibleObject<ROwater_fire>(objData, EN_FRONT);
					return true;
				}
				//火
				else if (objData.EqualObjectName(GetLevelObjectName(EN_RO_TYPE_FIRE)) == true)
				{
					//反転オブジェクトの生成
					NewReversibleObject<ROwater_fire>(objData, EN_BACK);
					return true;
				}
				//稼働、停止
				//稼働
				else if (objData.EqualObjectName(GetLevelObjectName(EN_RO_TYPE_RUNNING)) == true)
				{
					//反転オブジェクトの生成
					NewReversibleObject<nsReversibleObject::ROrunning_stop>(objData, EN_FRONT);
					return true;
				}
				//停止
				else if (objData.EqualObjectName(GetLevelObjectName(EN_RO_TYPE_STOP)) == true)
				{
					//反転オブジェクトの生成
					NewReversibleObject<nsReversibleObject::ROrunning_stop>(objData, EN_BACK);
					return true;
				}
				//釘、金槌
				//釘
				else if (objData.EqualObjectName(GetLevelObjectName(EN_RO_TYPE_NAIL)) == true)
				{
					//反転オブジェクトの生成
					NewReversibleObject<ROnail_hammer>(objData, EN_FRONT);
					return true;
				}
				//金槌
				else if (objData.EqualObjectName(GetLevelObjectName(EN_RO_TYPE_HAMMER)) == true)
				{
					//反転オブジェクトの生成
					NewReversibleObject<ROnail_hammer>(objData, EN_BACK);
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
					NewObstacleObject<OOflame>(objData);
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
					OObject->SetLeftOrRight(nsCommonData::EN_LEFT);
					return true;
				}
				//右
				else if (objData.EqualObjectName(GetLevelObjectName(OO_TYPE_ONEWAY_RIGHT)) == true)
				{
					//障害オブジェクトの生成
					OOoneway* OObject = NewObstacleObject<OOoneway>(objData);
					//左向きに設定
					OObject->SetLeftOrRight(EN_RIGHT);
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
				else if (objData.EqualObjectName(GetLevelObjectName(EN_OO_TYPE_UFO_CAPTURE)) == true)
				{
					//障害オブジェクトの生成
					//不透明モードで、オブジェクトの名前を付ける
					m_ufo = NewObstacleObject<nsLevelObject::nsObstacleObject::CUFO>(
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
					m_switch->SetEntityMaxTime(m_switchTime);
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
					NewReversibleObject<ROwater_fire>(objData, EN_BACK, MODE_TRANS);
					return true;
				}
				//釘、金槌（透明）
				//釘（透明）
				else if (objData.EqualObjectName(GetLevelObjectName(RO_TYPE_NAIL_TRANSPARENT)) == true)
				{
					//反転オブジェクトの生成
					//透明モード
					NewReversibleObject<ROnail_hammer>(objData, EN_FRONT, MODE_TRANS);
					return true;
				}
				//金槌（透明）
				else if (objData.EqualObjectName(GetLevelObjectName(RO_TYPE_HAMMER_TRANSPARENT)) == true)
				{
					//反転オブジェクトの生成
					//透明モード
					NewReversibleObject<ROnail_hammer>(objData, EN_BACK, MODE_TRANS);
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
					NewObstacleObject<OOflame>(objData, MODE_TRANS);
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

					//ウェイポイントを見るためのコード
					//if (InitedWayPoint == false)
					//{
					//	CModelRender* wayPointModel = NewGO<CModelRender>(0);
					//	wayPointModel->Init("Assets/modelData/yuka.tkm");
					//	wayPointModel->SetPosition(objData.position);
					//	wayPointModel->SetRotation(objData.rotation);
					//}
					return true;
				}
				return false;
				});

			//ウェイポイントを見るためのコード
			//InitedWayPoint = true;


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
			const nsTkLevel::LevelObjectData& objData,
			const nsCommonData::EN_FRONT_OR_BACK frontOrBack,
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
			const nsTkLevel::LevelObjectData& objData,
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
			//単体のオブジェクトを破棄
			DeleteGO(m_gameCamea);
			DeleteGO(m_pause);
			DeleteGO(m_sky);
			DeleteGO(m_tipsController);
			DeleteGO(m_capacityUI);


			//サウンドの破棄
			DeleteGO(m_bgmStage1);
			DeleteGO(m_bgmStage2);
			DeleteGO(m_loop_bgmStage1);
			DeleteGO(m_loop_bgmStage2);




			//レベルでロードしたオブジェクトを消去

			////////////////////////////////////////////////////////////
			// 新しいオブジェクトを作成時、ここでDeleteGOすること
			////////////////////////////////////////////////////////////

			//「基本オブジェクト」
			QueryGOs<nsMobyrinth::nsPlayer::Player>
				(GetGameObjectName(EN_GO_TYPE_PLAYER), [&](nsMobyrinth::nsPlayer::Player* player)->bool
				{
					DeleteGO(player);
					return true;
				}
			);
			QueryGOs<nsMobius::CMobius>
				(GetGameObjectName(EN_GO_TYPE_MOBIUS), [&](nsMobius::CMobius* mobius)->bool
				{
					DeleteGO(mobius);
					return true;
				}
			);


			//レベルオブジェクト
			//（ReversibleObject、ObstacleObjectのこと）
			//を全部消去
			CLevelObjectManager::GetInstance()->AllDeleteLOs();

		}


		//アップデート関数
		void IStageBase::Update()
		{
			//ゴールしているかどうかを調べる
			CheckGoal();

			//Start関数ではなくUpdate関数でBGMを初期化しているのは
			//ステージをロード中にBGMが再生しないようにするため。

			//BGMを初期化していなかったら
			if (!m_initedBGM)
			{
				//BGMを初期化する
				InitBGM();
			}

			//タイトル画面か？
			if (!m_isTitle)
			{
				//ゲーム画面
				//BGMインタラクティブを実行する
				BGMInteractive();
			}

			//ワイプインフラグが立っていたら
			if (m_wipeInFlag)
			{
				//ワイプが終了位置にいないか調べて、
				m_wipeInFlag = !g_graphicsEngine->GetHUD().IsWipeFinished();
				//終了位置まで来ていたら、ワイプの終了させる
				if (!m_wipeInFlag)
				{
					g_graphicsEngine->GetHUD().WipeEnd();
					m_gameCamea->SetWipeEndFlag(true);
				}
			}

			return;
		}

		//ポーズ中のみ呼ばれるアップデート関数
		void IStageBase::UpdateOnlyPaused()
		{
			//ポーズの状態を調べる

			//リトライが選択されたか？
			if (m_pause->GetRetryFlag())
			{
				//リトライ
				Retry();
			}
			//終了が選択されたか？
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
			//ワイプが終わっていか？
			if (!WipeOut())
			{
				//終わっていない
				//ここでreturn
				return;
			}


			//セーブデータの構造体
			nsSave::SSaveData saveData;
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
			//ワイプが終わっていか？
			if (!WipeOut())
			{
				//終わっていない
				//ここでreturn
				return;
			}

			//オーバーライドされているはずの処理を行う
			RetryStage();
			Release();	//リリース
		}

		/// <summary>
		///	終了した時の処理
		/// </summary>
		void IStageBase::Quit()
		{
			//ワイプが終わっていか？
			if (!WipeOut())
			{
				//終わっていない
				//ここでreturn
				return;
			}

			//タイトルの戻る
			GoTitle();
			Release();	//リリース
		}

		/// <summary>
		/// ゴールした時の処理
		/// </summary>
		void IStageBase::Goal()
		{
			//ゴールした後の時間を計測する	//デルタタイムを掛ける
			m_goalTimer += nsTimer::GameTime().GetFrameDeltaTime();

			//一定時間たったら
			if (m_goalTimer >= TIME_WAIT_GOAL)
			{
				//クリアする
				Clear();
			}
		}

		void IStageBase::CheckGoal()
		{
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

		}

		/// <summary>
		/// ワイプインする
		/// </summary>
		void IStageBase::WipeIn()
		{
			//ワイプインする
			g_graphicsEngine->GetHUD().WipeIn();
		}

		/// <summary>
		/// ワイプアウトする
		/// </summary>
		/// <returns>ワイプアウトが終了したか？</returns>
		bool IStageBase::WipeOut()
		{
			//ワイプの状態が、開始位置なら
			if (g_graphicsEngine->GetHUD().GetWipeSize() == 0.0f)
				//ワイプアウトする
				g_graphicsEngine->GetHUD().RandomWipeOut();

			//ワイプが終わったかどうか？
			return g_graphicsEngine->GetHUD().IsWipeFinished();

		}

		/// <summary>
		/// タイトルへ遷移する
		/// </summary>
		void IStageBase::GoTitle()
		{
			//タイトルを生成
			nsTitle::CTitle* title = NewGO<nsTitle::CTitle>(PRIORITY_FIRST);
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

			//この関数限定で
			//サウンドを使用可能にする
			using namespace nsSound;

			//BGMのサウンドキューを生成する
			m_bgmStage1 = NewGO<CSoundCue>(PRIORITY_FIRST);
			//BGMのサウンドキューを、waveファイルを指定して初期化する。
			m_bgmStage1->Init(SOUND_FILEPATH_BGM_STAGE_1);
			m_bgmStage1->SetSoundType(CSoundCue::enBGM);
			//BGMをループ再生をオンで再生する。
			m_bgmStage1->Play(true);
			//もしタイトル画面だった場合、BGM音量を０にする。
			if (m_isTitle) {
				m_bgmStage1->SetVolume(0.0f);
			}

			//BGMのサウンドキューを生成する
			m_bgmStage2 = NewGO<CSoundCue>(PRIORITY_FIRST);
			//BGMのサウンドキューを、waveファイルを指定して初期化する。
			m_bgmStage2->Init(SOUND_FILEPATH_BGM_STAGE_2);
			m_bgmStage2->SetSoundType(CSoundCue::enBGM);
			//BGMをループ再生をオンで再生する。
			m_bgmStage2->Play(true);
			//BGM音量を０にする。
			m_bgmStage2->SetVolume(0.0f);


			//BGMのサウンドキューを生成する
			m_loop_bgmStage1 = NewGO<CSoundCue>(PRIORITY_FIRST);
			//BGMのサウンドキューを、waveファイルを指定して初期化する。
			m_loop_bgmStage1->Init(SOUND_FILEPATH_BGM_STAGE_1_LOOP);
			m_loop_bgmStage1->SetSoundType(CSoundCue::enBGM);
			//BGM音量を０にする。
			m_loop_bgmStage1->SetVolume(0.0f);

			//BGMのサウンドキューを生成する
			m_loop_bgmStage2 = NewGO<CSoundCue>(PRIORITY_FIRST);
			//BGMのサウンドキューを、waveファイルを指定して初期化する。
			m_loop_bgmStage2->Init(SOUND_FILEPATH_BGM_STAGE_2_LOOP);
			m_loop_bgmStage2->SetSoundType(CSoundCue::enBGM);
			//BGM音量を０にする。
			m_loop_bgmStage2->SetVolume(0.0f);

			m_initedBGM = true;
		}

		void IStageBase::BGMInteractive()
		{
			//プレイヤーが見つかっていないか？
			if (!m_player)
			{
				//見つかっていない
				//プレイヤーを探す
				m_player = FindGO<nsMobyrinth::nsPlayer::Player>(GetGameObjectName(EN_GO_TYPE_PLAYER));
				//まだ見つかっていなかったら
				if (!m_player)
					//下の処理を実行しない
					return;
			}

			//プレイヤーのウェイポイント
			const int lpIndex = m_player->GetLeftPointIndex();

			//プレイヤーが表側にいるか裏側にいるかを調べる
			int playerFrontOrBackSide = CLevelObjectManager::GetInstance()->CheckFrontOrBackSide(lpIndex);

			//プレイヤーが表側にいるとき
			if (playerFrontOrBackSide == EN_FRONT_SIDE) {
				if (m_bgmStage1->GetVolume() < 1.0f) {
					m_bgmStage1->
						SetVolume(m_bgmStage1->GetVolume() + SOUND_VOLUME_INC_OR_DEC_BGM_INTERACTIVE);
					m_bgmStage2->
						SetVolume(m_bgmStage2->GetVolume() - SOUND_VOLUME_INC_OR_DEC_BGM_INTERACTIVE);
				}
			}

			//プレイヤーが裏側にいるとき
			if (playerFrontOrBackSide == EN_BACK_SIDE) {
				if (m_bgmStage2->GetVolume() < 1.0f) {
					m_bgmStage2->
						SetVolume(m_bgmStage2->GetVolume() + SOUND_VOLUME_INC_OR_DEC_BGM_INTERACTIVE);
					m_bgmStage1->
						SetVolume(m_bgmStage1->GetVolume() - SOUND_VOLUME_INC_OR_DEC_BGM_INTERACTIVE);
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

	}
}