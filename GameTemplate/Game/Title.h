#pragma once
#include "FontRender.h"
#include "SpriteRender.h"
#include "Level2D.h"
#include "SoundSource.h"

//背景
#include "stage_title.h"
#include "Mobius.h"
#include "Player.h"

//ステージ
#include "stage_kari.h"
#include "stage_proto01.h"
#include "stage_proto02.h"

/// <summary>
/// タイトルクラス
/// </summary>
class Title : public IGameObject
{
public:		//自動で呼ばれるメンバ関数
	bool Start()override final;		//スタート関数
	~Title();						//デストラクタ
	void Update()override final;	//アップデート関数

private:	//privateなメンバ関数

	void TitleScreen();	//タイトル画面
	void StageSelect();	//ステージセレクト
	void Release()		//自身のオブジェクトを破棄する関数
	{
		DeleteGO(this);
	}

private:	//データメンバ

	/// <summary>
	/// ステージのステート（状態）
	/// </summary>
	enum EnState
	{
		enTitleScreen,	//タイトル画面の状態
		enStageSelect,	//ステージセレクトの状態
	};
	int m_stageState = enTitleScreen;	//現在のステージのステート（状態）

	/// <summary>
	/// ステージの番号
	/// 新しいステージを作った場合、ここに番号を追加
	/// </summary>
	enum EnStageList
	{
		enTitle,
		enStage_kari,
		enStageProto01,
		enStageProto02,
		enStageNum,
	};

	int m_stageSelectState = enStage_kari;	//現在のステージセレクトのステート

	CFontRender* m_titleFR = nullptr;					//フォントレンダー
	CFontRender* m_pushAButtonFR = nullptr;				//フォントレンダー
	CFontRender* m_stageName[enStageNum] = { nullptr };	//フォントレンダー
	CFontRender* m_arrow = nullptr;						//フォントレンダー
	bool m_buttonFlag = true;							//ボタンを押すことができるか？
														//（連続入力防止用）
	CLevel2D m_level2D;									//スプライト用のレベルクラス
	std::list<CSpriteRender*> m_spriteRenders;
	CSpriteRender* m_title = nullptr;

private: //背景
	Mobius* m_mobius = nullptr;
	Player* m_player = nullptr;

	stage_title* m_stageTitle = nullptr;


	//デバック用
	//後で消す
	CSoundSource* m_soundSource = nullptr;				//サウンドソースクラス
	CSoundSource* m_bgmSS = nullptr;					//BGMのサウンドソース

	//デバック用ここまで
};

