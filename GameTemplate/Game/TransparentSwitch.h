#pragma once
#include "ObstacleObject.h"
#include "FontRender.h"
#include "SpriteRender.h"
#include "GameCamera.h"
#include "UFO.h"
#include "SoundCue.h"
#include "effect/Effect.h"
#include "Pause.h"


class OOTransparentSwitch : public CObstacleObject
{
public:		//オーバーライドしてほしいメンバ関数
	virtual bool StartSub();	//スタート関数
	virtual void UpdateSub();	//アップデート関数

public:
	OOTransparentSwitch();		//コンストラクタ
	~OOTransparentSwitch();		//デストラクタ

public: //Set関数
	/// <summary>
	/// リセットタイマーが作動したときの値を変更
	/// </summary>
	/// <param name="i">リセットタイマーが作動したときの値</param>
	void SetResetTimerStartValue(float f)
	{
		m_resetTimerStartValue = f;
	}

	/// <summary>
	/// オブジェクトを半透明にする
	/// </summary>
	void ChangeTransparent();

	/// <summary>
	/// オブジェクトを実体にする
	/// </summary>
	void ChangeEntity();

	/// <summary>
	/// スイッチのタイマーがカウントするか？を設定する
	/// </summary>
	/// <param name="timerCountFlag">タイマーがカウントするか？</param>
	void SetTimerCountFlag(const bool timerCountFlag)
	{
		m_timerCountFlag = timerCountFlag;
	}


private:	//privateなメンバ関数

	/// <summary>
	/// タイマーのフォントの更新
	/// </summary>
	void UpdateTimerFR();

	/// <summary>
	/// タイマーのフォントのパラメーターを設定する
	/// </summary>
	/// <param name="itr">タイマーのフォントのイテレーター</param>
	/// <param name="levelObject">対応する透明オブジェクト</param>
	void SetTimerFRParam
	(ILevelObjectBase* levelObject);

	/// <summary>
	/// 透明、実体化を切り替える処理
	/// </summary>
	void Switching();

private: //メンバ変数
	float m_resetTimer = 0.0f; //オブジェクトの状態をリセットするまでのカウントに使用するリセットタイマー
	float m_resetTimerStartValue = 10.0f; //リセットタイマーが作動したときの値を保存する変数
	bool m_flagSwitchOn = false; //スイッチが押されて、透明オブジェクトを持ち上げられる状態になっているかのフラグ
	bool m_timerFRIsActive = false;				//タイマーのフォントがアクティブか？
	Vector4 m_fontColor;						//フォントのカラー
	Vector4 m_normalColor;						//通常のフォントのカラー
	Vector4 m_blinkColor;						//点滅時のフォントのカラー
	float m_blinkTimer = FLT_MAX;				//点滅タイマー、最初は最大値を入れておく
	CModelRender* m_modelRender = nullptr;		//ボタンが押されたとき用のモデルレンダラー
	CGameCamera* m_gameCamera = nullptr;			//カメラのポインタ
	CUFO* m_ufo = nullptr;						//UFOのポインタ
	bool m_switchingFlag = false;				//スイッチの切り替え中か？
	float m_switchingTimer = 0.0f;				//スイッチの切り替え中のタイマー
	//CSpriteRender* m_fadeSR = nullptr;			//フェードイン、アウトで使うスプライトレンダラー
	CSoundCue* m_buttonpushSE = nullptr;		//m_buttonpushSEのサウンドキュー
	CSoundCue* m_activationSE = nullptr;		//m_activationSEのサウンドキュー
	CSoundCue* m_invalidationSE = nullptr;		//m_invalidationSEのサウンドキュー
	Effect* m_activation = nullptr;				//オブジェクトが有効化した時のエフェクト
	CPause* m_pause = nullptr;					//ポーズクラスのポインタ
	CSoundCue* m_timerSC = nullptr;				//タイマーのサウンドキュー
	CSoundCue* m_timerWarningSC = nullptr;		//警告タイマーのサウンドキュー
	bool m_timerCountFlag = true;				//スイッチのタイマーがカウントするか？
};

