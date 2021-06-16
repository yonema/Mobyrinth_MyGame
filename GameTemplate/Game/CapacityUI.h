#pragma once
#include "SpriteRender.h"
#include "FontRender.h"
#include "LevelObjectManager.h"
#include "SoundCue.h"
#include "Player.h"

/// <summary>
/// キャパシティのUI表示クラス
/// </summary>
class CCapacityUI : public IGameObject
{
public:		//自動で呼ばれるメンバ関数

	bool Start() override final;	//スタート関数
	~CCapacityUI();					//デストラクタ
	void Update() override final;	//アップデート関数

public:		//メンバ関数S

	/// <summary>
	/// タイトルか？を設定する
	/// </summary>
	/// <param name="titleFlag">タイトルか？</param>
	void SetTitleFlag(const bool titleFlag)
	{
		m_titleFlag = titleFlag;
	}

private:	//privateなメンバ関数

	//スプライトの初期化
	void InitSprite();

	//フォントの初期化
	void InitFont();

	/// <summary>
	/// どの演出を起こすかチェックする
	/// </summary>
	/// <param name="num">表側と裏側のアイテムの数</param>
	/// <param name="maxNum">表側と裏側のアイテムの最大数</param>
	void CheckDirecting(const int* num);

	//演出の割り振り
	void Directing();
	
	/// <summary>
	/// 増えた演出
	/// </summary>
	/// <param name="frontOrBackSide">表側か裏側か？</param>
	void Increased(const int frontOrBackSide);

	/// <summary>
	/// 減った演出
	/// </summary>
	/// <param name="frontOrBackSide">表側か裏側か？</param>
	void Decreased(const int frontOrBackSide);

	/// <summary>
	/// オーバーした演出
	/// </summary>
	/// <param name="frontOrBackSide">表側か裏側か？</param>
	void Over(const int frontOrBackSide);

	/// <summary>
	/// 通常の値に戻す
	/// </summary>
	void DefaultParam(const int frontOrBackSide);

	void CheckActiveFontColor();


private:	//列挙型
	
	//演出のステートの列挙型
	enum EnDirectingState
	{
		enNormal,		//通常
		enIncreased,	//増えた
		enDecreased,	//減った
		enOver,			//オーバーした
	};

	//表側と裏側の列挙型
	enum EnFrontOrBackSide
	{
		EN_FRONT_SIDE,			//表側
		EN_BACK_SIDE,				//裏側
		enFrontAndBackSideNum,	//表側と裏側の数
	};

private:	//データメンバ

	CSpriteRender* m_capacityDisplaySR = nullptr;	//キャパシティを表示するスプライト
	CFontRender* m_RONumFR[enFrontAndBackSideNum];	//表側と裏側にあるアイテムの数のフォント
	CFontRender* m_capacityFR[enFrontAndBackSideNum];	//キャパシティのフォント
	Vector2 m_capacityPos[enFrontAndBackSideNum];	//キャパシティのフォントの座標
	float m_diffCapacityToNum = 0.0f;				//キャパシティからアイテムの数のフォントへの差
	Vector4 m_defaultFontColor[enFrontAndBackSideNum];	//フォントのカラー
	Vector4 m_activeFontColor;						//プレイヤーがいる側のフォントのカラー
	Vector4 m_inactiveFontColor;					//プレイヤーがいない側のフォントのカラー
	int m_oldRONum[enFrontAndBackSideNum];			//前回の表側と裏側にあるアイテムの数
	bool m_firstFrame = true;						//最初のフレームか？
	bool m_titleFlag = false;						//タイトルか？
	int m_directingState[enFrontAndBackSideNum] = { enNormal };	//演出のステート
	float m_timer[enFrontAndBackSideNum] = { 0.0f };	//タイマー
	int m_maxCapacityNum[enFrontAndBackSideNum] = { 5,5 };	//キャパシティの最大値
	float m_defaultScale = 1.0f;					//通常の拡大
	CSoundCue* m_capacity_over = nullptr;			//キャパオーバーの音
	Player* m_player = nullptr;						//プレイヤーのポインタ
	//プレイヤーが表側にいるか裏側にいるか？
	int m_playerFrontOrBackSide = EN_FRONT_SIDE;
};

