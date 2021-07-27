#pragma once

//基本的に必要
#include "StageBaseConstData.h"
#include "GameCamera.h"
#include "SoundCue.h"
#include "Sky.h"

//ポーズ画面用
#include "Pause.h"

//Tips用
#include "TipsController.h"

//キャパシティのUI用
#include "CapacityUI.h"

//セーブデータ用
#include "Save.h"



//レベルのロードで必要
#include "Level.h"
#include "LevelObjectManager.h"

//基本オブジェクト
#include "Player.h"
#include "Mobius.h"

//「反転オブジェクト」ReversibleObject
#include "ROwater_fire.h"
#include "ROrunning_stop.h"
#include "ROnail_hammer.h"
#include "ROleft_right.h"
#include "ROkey_padlock.h"

//「障害オブジェクト」ObstacleObject
#include "OOflame.h"
#include "OOwall.h"
#include "OOgoal.h"
#include "OOpadlock.h"
#include "OObox.h"
#include "OOTransparentSwitch.h"
#include "OOoneway.h"
#include "OOReverseALL.h"

//「UFO」
#include "UFO.h"

//サウンド
#include "SoundCue.h"


/// <summary>
///	ステージベースクラス
/// </summary>
class IStageBase : public IGameObject
{
public:		//自動で呼ばれるメンバ関数
	bool Start()override final;				//スタート関数
	~IStageBase();							//デストラクタ
	void Update()override final;			//アップデート関数
	void UpdateOnlyPaused()override final;	//ポーズ中のみ呼ばれるアップデート関数

protected:	//オーバーライドしてほしいメンバ関数

	/// <summary>
	/// オーバーライドしほしいスタート関数
	/// </summary>
	/// <returns></returns>
	virtual bool StartSub() = 0 { return true; };

	/// <summary>
	/// オーバーライドしてほしい、
	/// リトライした時の処理
	/// </summary>
	virtual void RetryStage() = 0 {};

public: //メンバ関数

	/**
	 * @brief タイトルモードにする
	 * @attention Start関数が呼ばれる前に呼ばないと意味がない。
	*/
	void TitleMode()
	{
		m_isTitle = true;
	}


	/// <summary>
	/// ワイプインするかどうかのフラグを設定
	/// </summary>
	/// <param name="wipeInFlag">ワイプインする？</param>
	void SetWipeInFlag(const bool wipeInFlag)
	{
		m_wipeInFlag = wipeInFlag;
	}

	/// <summary>
	/// 一番高いクリアしたステージの番号を戻す
	/// </summary>
	/// <returns> 一番高いクリアしたステージ</returns>
	const int GetHighestClearStageNum() const
	{
		return m_highestClearStageNum;
	}

	/// <summary>
	/// 一番高いクリアしたステージの番号を設定する
	/// </summary>
	/// <returns> 一番高いクリアしたステージ</returns>
	void SetHighestClearStageNum(const int clearStageNum)
	{
		m_highestClearStageNum = clearStageNum;
	}

	/// <summary>
	/// スイッチのタイムを設定
	/// </summary>
	/// <param name="switchTime"></param>
	void SetSwitchTime(const float switchTime)
	{
		m_switchTime = switchTime;
	}

	/// <summary>
	/// スイッチのタイムの取得
	/// </summary>
	/// <returns></returns>
	const float GetSwitchTime() const
	{
		return m_switchTime;
	}

protected:	//protectedなメンバ関数
	/// <summary>
	/// レベルのロード
	/// </summary>
	/// <param name="filePath">tklのファイルパス</param>
	void LoadLevel(const char* tklFilePath);

	/// <summary>
	/// リリース関数
	/// </summary>
	void Release()
	{
		DeleteGO(this);
	}

	/// <summary>
	/// ステージの番号を設定する
	/// </summary>
	/// <param name="stageNum">ステージの番号</param>
	void SetStageNum(const int stageNum)
	{
		m_stageNum = stageNum;
	}

	/// <summary>
	/// ステージの番号を取得する
	/// </summary>
	/// <returns>ステージの番号</returns>
	const int GetStageNum() const
	{
		return m_stageNum;
	}



private:	//privateなメンバ関数

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
	C* NewReversibleObject(
		const LevelObjectData& objData,
		const bool frontOrBack,
		const bool transparent = false,
		const char* const objectName = NAME_EMPTY,
		const int priority = PRIORITY_FIRST
	);

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
	C* NewObstacleObject(
		const LevelObjectData& objData,
		const bool transparent = false,
		const char* const objectName = NAME_EMPTY,
		const int priority = PRIORITY_FIRST
	);

	/// <summary>
	/// クリアした時の処理
	/// </summary>
	void Clear();

	/// <summary>
	/// リトライした時の処理
	/// </summary>
	void Retry();

	/// <summary>
	///	終了した時の処理
	/// </summary>
	void Quit();

	/// <summary>
	/// ゴールした時の処理
	/// </summary>
	void Goal();

	/// <summary>
	/// ゴールしているかどうか調べる
	/// </summary>
	void CheckGoal();

	/// <summary>
	/// タイトルへ遷移する
	/// </summary>
	void GoTitle();

	/// <summary>
	/// ワイプインする
	/// </summary>
	void WipeIn();

	/// <summary>
	/// ワイプアウトする
	/// </summary>
	/// <returns>ワイプアウトが終了したか？</returns>
	bool WipeOut();

	void BGMInteractive();

	/// <summary>
	/// BGMの初期化
	/// </summary>
	void InitBGM();

private:	//データメンバ

	/*
	* レベルロード関連
	*/

	CLevel m_level;									//レベルロード用クラス
	CSky* m_sky = nullptr;							//空クラスのポインタ
	CGameCamera* m_gameCamea = nullptr;				//ゲームカメラ
	Player* m_player = nullptr;						//プレイヤークラスのポインタ
	CPause* m_pause = nullptr;						//ポーズクラスのポインタ
	OOgoal* m_goal = nullptr;						//ゴールクラスのポインタ
	OOTransparentSwitch* m_switch = nullptr;		//スイッチのポインタ
	CUFO* m_ufo = nullptr;							//UFOのポインタ



	float m_goalTimer = 0;							//ゴールした後の待ち時間をはかる

	bool m_isTitle = false;							//タイトル画面か？

	bool m_wipeInFlag = false;						//ワイプインする？

	float m_switchTime = stageBaseConstData::SWITCH_TIME_DEFAULT;	//スイッチのタイム

	/*
	* キャパシティ関連
	*/
	CTipsController* m_tipsController = nullptr;	//Tipsのコントロ―ルクラス
	CCapacityUI* m_capacityUI = nullptr;			//キャパシティのUIクラス


	/*
	* セーブ関連
	*/
	CSave m_save;									//セーブデータクラス
	int m_highestClearStageNum = -1;				//一番高いクリアステージ
	int m_stageNum = -1;							//ステージの番号


	/*
	* サウンド関連
	*/
	CSoundCue* m_bgmStage1 = nullptr;				//BGMのサウンドキュー
	CSoundCue* m_bgmStage2 = nullptr;				//BGMのサウンドキュー
	CSoundCue* m_loop_bgmStage1 = nullptr;			//BGMのサウンドキュー
	CSoundCue* m_loop_bgmStage2 = nullptr;			//BGMのサウンドキュー
	bool m_check_loop1 = false;						//ループ確認
	bool m_check_loop2 = false;						//ループ確認
	bool m_initedBGM = false;						//BGMが初期化済みか？
};

