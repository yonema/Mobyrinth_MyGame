#pragma once
#include "StartDirectingConstData.h"
#include "Player.h"
#include "Pause.h"
#include "UFO.h"
class CGameCamera;

/**
 * @brief 開始演出クラス
*/
class CStartDirecting : private Noncopyable
{
public:		//自動で呼ばれるメンバ関数
	CStartDirecting() {};				//コンストラクタ
	~CStartDirecting() {};				//デストラクタ

private:	//privateなメンバ関数

	/**
	 * @brief ステージの周りをくるくる回る処理
	*/
	void GoAroundStage();

	/**
	 * @brief フェードの処理
	*/
	void FadeDirectingCamera();

	/**
	 * @brief プレイヤーの演出時の落下処理
	*/
	void FallPlayer();

public:		//メンバ関数

	/**
	 * @brief 開始演出の更新。使うときにアップデート関数で呼ぶ。
	*/
	void StartDirectingUpdate();

	/**
	 * @brief ステージの開始演出を行っているか？を得る
	 * @return 開始演出を行っているか？
	*/
	bool GetStartDirectingFlag()const
	{
		return m_startDirectingFlag;
	}

	/**
	 * @brief ステージの開始演出を行っているか？を設定する
	 * @param [in] startDirecting ステージの開始演出を行っているか？
	*/
	void SetStartDirectingFlag(const bool startDirecting)
	{
		m_startDirectingFlag = startDirecting;
	}

	/**
	 * @brief ワイプが終わったか？を設定
	 * @param [in] wipeEndFlag ワイプが終わったか？
	*/
	void SetWipeEndFlag(const bool wipeEndFlag)
	{
		m_wipeEndFlag = wipeEndFlag;
	}

	/**
	 * @brief ゲームカメラのアクセスをセット
	 * @param [in] gameCamera ゲームカメラのアドレス
	*/
	void SetGameCamera(CGameCamera* gameCamera)
	{
		m_gameCamera = gameCamera;
	}

private:	//データメンバ

	/*
	* フラグ関連
	*/
	bool m_startDirectingFlag = true;	//開始演出中か？（GameCameraクラスで使用）
	bool m_wipeEndFlag = false;			//ワイプが終わった？
	bool m_goAroundStageFlag = true;	//ステージの周りを回るか？
	bool m_fadeFlag = false;			//フェードするか？
	bool m_fallPlayerFlag = false;		//プレイヤーが落ちてくる演出を行うか？

	/*
	* タイマー関連
	*/
	float m_firstWaitTimer = 0;			//演出開始までのタイマー
	float m_fadeTimer = 0.0f;			//フェード中のタイマー
	float m_waitTimerAfterFell = 0.0f;	//落下後から開始までの時間


	float m_goAroundStageAngle = 0.0f;	//開始演出中にステージの周りを回っている角度

	Player* m_player = nullptr;			//プレイヤー
	CGameCamera* m_gameCamera = nullptr;//ゲームカメラ
};

