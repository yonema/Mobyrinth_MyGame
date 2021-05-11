#pragma once
#include "FontRender.h"
#include "SpriteRender.h"
#include "Level2D.h"
#include "SoundCue.h"

//背景
#include "stage_title.h"
#include "Mobius.h"
#include "Player.h"

//ステージ
//#include "stage_kari.h"
//#include "stage_proto01.h"
//#include "stage_proto02.h"
#include "Stage.h"

/// <summary>
/// タイトルクラス
/// </summary>
class Title : public IGameObject
{
public:		//自動で呼ばれるメンバ関数
	bool Start()override final;		//スタート関数
	~Title();						//デストラクタ
	void Update()override final;	//アップデート関数

public:		//メンバ関数
	void SetWipeInFlag(const bool wipeInFlag)
	{
		m_wipeInFlag = wipeInFlag;
	}

private:	//privateなメンバ関数

	void InitBGM();			//BGMの初期化
	void TitleScreen();		//タイトル画面
	void StartTitle();		//起動時の演出
	void StageSelect();		//ステージセレクト
	void StageDecision();	//ステージを決定した時の処理
	void Release()			//自身のオブジェクトを破棄する関数
	{
		DeleteGO(this);
	}

private:	//列挙型

	/// <summary>
	/// ステージのステート（状態）
	/// </summary>
	enum EnState
	{
		enStartTitle,		//起動時演出の状態
		enTitleScreen,		//タイトル画面の状態
		enStageSelect,		//ステージセレクトの状態
		enStageDecision,	//ステージを決定した状態
	};
	int m_stageState = enStartTitle;	//現在のステージのステート（状態）

	/// <summary>
	/// ステージの番号
	/// 新しいステージを作った場合、ここに番号を追加
	/// </summary>
	enum EnStageList
	{
		enStage1,		//ステージ1
		enStage2,		//ステージ2
		enStage3,		//ステージ3
		enStage4,		//ステージ4
		enStage5,		//ステージ5
		enStage6,		//ステージ6
		enStage7,		//ステージ7
		enStage8,		//ステージ8
		enStage9,		//ステージ9
		enStageNum,		//ステージの総数
	};

private:	//データメンバ

	int m_stageSelectState = enStage1;	//現在のステージセレクトのステート

	bool m_buttonFlag = true;				//ボタンを押すことができるか？
											//（連続入力防止用）
	CLevel2D m_level2D;						//スプライト用のレベルクラス
	bool m_wipeInFlag = false;				//ワイプインするか？
	Vector3 m_stageIconToCursorVec = g_vec3Zero;	//ステージのアイコンからカーソルへのベクトル

private: //背景
	Mobius* m_mobius = nullptr;
	Player* m_player = nullptr;

	stage_title* m_stageTitle = nullptr;

private: //画像データ
	CSpriteRender* m_title = nullptr;
	CSpriteRender* m_pressAButton = nullptr;
	CSpriteRender* m_cursor = nullptr;
	CSpriteRender* m_stageName[enStageNum] = { nullptr };
	CSpriteRender* m_stageClear[enStageNum] = { nullptr };
	CSpriteRender* m_stageSelection = nullptr;
	CSpriteRender* m_stageSelectionBase = nullptr;

	bool m_blinkingFlag = true; //m_pressAButtonの透過処理に使用

private: //BGMデータ
	CSoundCue* m_bgmTitle = nullptr;				//BGMのサウンドキュー
	bool m_initedBGM = false;						//BGMが初期化済みか？

	CSoundCue* m_selectSE = nullptr;	//selectSEのサウンドキュー
	CSoundCue* m_buttonASE = nullptr;	//buttonASEのサウンドキュー
	CSoundCue* m_buttonBSE = nullptr;	//buttonBSEのサウンドキュー

private: //起動時演出の変数
	int m_countStartTitle = 0;
};

