#pragma once
#include "ObstacleObject.h"
#include "FontRender.h"
#include "SpriteRender.h"
#include "GameCamera.h"
#include "UFO.h"
#include "SoundCue.h"
#include "effect/Effect.h"

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
	void ChangeTransparent()
	{
		//レベルオブジェクトを取ってくる
		std::vector<ILevelObjectBase*> levelObjects
			= CLevelObjectManager::GetInstance()->GetLevelObjects();
		//全てのレベルオブジェクトに検索
		for (int i = 0; i < levelObjects.size(); i++)
		{
			//透明オブジェクトではない場合、次のオブジェクトを検索
			if (levelObjects[i]->GetFlagTransparentObject() == false) {
				continue;
			}

			//モデルの参照を得てから、SetMulColor()を呼ぶ
			//Obstacleの場合は無駄に二回呼ばれるけど、我慢しよう。
			levelObjects[i]->GetModelRender(CReversibleObject::enFront)->SetMulColor({ 1.0f,1.0f,1.0f,0.5f });
			levelObjects[i]->GetModelRender(CReversibleObject::enBack)->SetMulColor({ 1.0f,1.0f,1.0f,0.5f });

			levelObjects[i]->TransparentSwitchOff();
		}
	}

	/// <summary>
	/// オブジェクトを実体にする
	/// </summary>
	void ChangeEntity()
	{
		//レベルオブジェクトを取ってくる
		std::vector<ILevelObjectBase*> levelObjects
			= CLevelObjectManager::GetInstance()->GetLevelObjects();

		//タイマーのフォントのイテレーター
		std::list<CFontRender*>::iterator itr = m_timerFR.begin();

		//全てのレベルオブジェクトに検索
		for (int i = 0; i < levelObjects.size(); i++)
		{
			//透明オブジェクトではない場合、次のオブジェクトを検索
			if (levelObjects[i]->GetFlagTransparentObject() == false) {
				continue;
			}

			//モデルの参照を得てから、SetMulColor()を呼ぶ
			//Obstacleの場合は無駄に二回呼ばれるけど、我慢しよう。
			levelObjects[i]->GetModelRender(CReversibleObject::enFront)->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });
			levelObjects[i]->GetModelRender(CReversibleObject::enBack)->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });

			levelObjects[i]->TransparentSwitchOn();

			//タイマーのフォントのパラメータを設定する
			SetTimerFRParam(itr, levelObjects[i]);

			//イテレーターを進める
			itr++;

		}
	}

	/// <summary>
	/// タイマーのフォントが何個いるのか設定する
	/// </summary>
	/// <param name="num">何個</param>
	void SetTimerFRNum(const int num);

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
	(std::list<CFontRender*>::iterator itr, const ILevelObjectBase* levelObject);

	void Switching();

private: //メンバ変数
	float m_resetTimer = 0.0f; //オブジェクトの状態をリセットするまでのカウントに使用するリセットタイマー
	float m_resetTimerStartValue = 10.0f; //リセットタイマーが作動したときの値を保存する変数
	bool m_flagSwitchOn = false; //スイッチが押されて、透明オブジェクトを持ち上げられる状態になっているかのフラグ
	std::list<CFontRender*> m_timerFR;			//タイマーのフォント
	Vector4 m_fontColor;						//フォントのカラー
	Vector4 m_normalColor;						//通常のフォントのカラー
	Vector4 m_blinkColor;						//点滅時のフォントのカラー
	float m_blinkTimer = FLT_MAX;				//点滅タイマー、最初は最大値を入れておく
	CModelRender* m_modelRender = nullptr;		//ボタンが押されたとき用のモデルレンダラー
	GameCamera* m_gameCamera = nullptr;			//カメラのポインタ
	CUFO* m_ufo = nullptr;						//UFOのポインタ
	bool m_switchingFlag = false;				//スイッチの切り替え中か？
	float m_switchingTimer = 0.0f;				//スイッチの切り替え中のタイマー
	CSpriteRender* m_fadeSR = nullptr;			//フェードイン、アウトで使うスプライトレンダラー
	CSoundCue* m_buttonpushSE = nullptr;		//m_buttonpushSEのサウンドキュー
	CSoundCue* m_activationSE = nullptr;		//m_activationSEのサウンドキュー
	CSoundCue* m_invalidationSE = nullptr;		//m_invalidationSEのサウンドキュー
	Effect* m_activation = nullptr;				//オブジェクトが有効化した時のエフェクト
};

