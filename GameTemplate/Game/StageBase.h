#pragma once

//基本的に必要
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
#include "ROmizu_kori.h"
#include "RObird_fish.h"
#include "ROrunning_stop.h"
#include "ROwire_string.h"
#include "ROnail_bar.h"
#include "ROaxe_pickaxe.h"
#include "ROkeymold_empty.h"
#include "ROleft_right.h"

//「障害オブジェクト」ObstacleObject
#include "OObigFire.h"
#include "OOwall.h"
#include "OOgoal.h"
#include "OOpadlock.h"
#include "OObox.h"
#include "TransparentSwitch.h"
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

private:	//privateなメンバ関数

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

public: //インライン関数
	void SetStartUpStartDirecting(const bool b)
	{
		m_startUpStartDirecting = b;
	}
	void SetTitlePlayer(const bool b)
	{
		m_titlePlayer = b;
	}
	void SetStartBGM(const bool b)
	{
		m_startBGM = b;
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


private:	//データメンバ
	CLevel m_level;									//レベルロード用クラス
	Player* m_player = nullptr;						//プレイヤークラスのポインタ
	CPause* m_pause = nullptr;						//ポーズクラスのポインタ
	OOgoal* m_goal = nullptr;						//ゴールクラスのポインタ
	CSky* m_sky = nullptr;							//空クラスのポインタ
	float m_goalTimer = 0;							//ゴールした後の待ち時間をはかる

	GameCamera* m_gameCamea = nullptr;				//ゲームカメラ

	StartDirecting* m_startDirecting = nullptr;
	bool m_startUpStartDirecting = true;
	bool m_titlePlayer = false;	

	CSoundCue* m_bgmStage1 = nullptr;				//BGMのサウンドキュー
	CSoundCue* m_bgmStage2 = nullptr;				//BGMのサウンドキュー
	CSoundCue* m_loop_bgmStage1 = nullptr;			//BGMのサウンドキュー
	CSoundCue* m_loop_bgmStage2 = nullptr;			//BGMのサウンドキュー
	bool m_check_loop1 = false;						//ループ確認
	bool m_check_loop2 = false;						//ループ確認
	bool m_startBGM = true;							//タイトル画面のBGM再生のフラグ
	bool m_initedBGM = false;						//BGMが初期化済みか？
	bool m_wipeInFlag = false;						//ワイプインする？

	CTipsController* m_tipsController = nullptr;	//Tipsのコントロ―ルクラス
	CCapacityUI* m_capacityUI = nullptr;			//キャパシティのUIクラス
	CSoundCue* m_goalSE = nullptr;					//m_goalSEのサウンドキュー
	int m_goalSEcount = 0;							//m_goalSEのサポート

	CSave m_save;									//セーブデータクラス
	int m_highestClearStageNum = -1;				//一番高いクリアステージ
	int m_stageNum = -1;							//ステージの番号
	
	CSpriteRender* m_goalSR = nullptr;				//ゴールのスプライト

};

