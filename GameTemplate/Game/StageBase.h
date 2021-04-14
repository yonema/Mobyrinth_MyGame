#pragma once

//基本的に必要
#include "DirectionLight.h"
#include "GameCamera.h"
#include "SoundCue.h"

//ポーズ画面用
#include "Pause.h"

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

//「障害オブジェクト」ObstacleObject
#include "OObigFire.h"
#include "OOwall.h"
#include "OOgoal.h"
#include "OOpadlock.h"
#include "OObox.h"



class IStageBase : public IGameObject
{
public:
	bool Start()override final;
	~IStageBase();
	void Update()override final;
	void UpdateOnlyPaused()override final;

private:
	void Clear();
	void Retry();
	void Quit();

	void Goal();

	void BGMInteractive();

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

protected:
	/// <summary>
	/// レベルのロード
	/// </summary>
	/// <param name="filePath">tklのファイルパス</param>
	void LoadLevel(const char* tklFilePath);
	CDirectionLight* GetStageDirectionLight()
	{
		return m_stageDirectionLight;
	}
	virtual bool StartSub() = 0 { return true; };
	virtual void RetryStage() = 0 {};
	void Release()
	{
		DeleteGO(this);
	}
private:
	CLevel m_level;
	CDirectionLight* m_stageDirectionLight = nullptr;
	CPause* m_pause = nullptr;
	OOgoal* m_goal = nullptr;
	int m_goalCounter = 0;

	StartDirecting* m_startDirecting = nullptr;
	bool m_startUpStartDirecting = true;
	bool m_titlePlayer = false;


private:
	CSoundCue* m_bgmStage1 = nullptr;				//BGMのサウンドキュー
	CSoundCue* m_bgmStage2 = nullptr;				//BGMのサウンドキュー
	CSoundCue* m_loop_bgmStage1 = nullptr;				//BGMのサウンドキュー
	CSoundCue* m_loop_bgmStage2 = nullptr;				//BGMのサウンドキュー
	bool m_check_loop1 = false; //ループ確認
	bool m_check_loop2 = false; //ループ確認
	bool m_startBGM = true; //タイトル画面のBGM再生のフラグ
};

