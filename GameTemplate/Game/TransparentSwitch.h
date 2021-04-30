#pragma once
#include "ObstacleObject.h"

class OOTransparentSwitch : public CObstacleObject
{
public:		//オーバーライドしてほしいメンバ関数
	virtual bool StartSub();	//スタート関数
	virtual void UpdateSub();	//アップデート関数

public: //Set関数
	/// <summary>
	/// リセットタイマーが作動したときの値を変更
	/// </summary>
	/// <param name="i">リセットタイマーが作動したときの値</param>
	void SetResetTimerStartValue(int i)
	{
		m_resetTimerStartValue = i;
	}

private: //メンバ変数
	int m_resetTimer = 0; //オブジェクトの状態をリセットするまでのカウントに使用するリセットタイマー
	int m_resetTimerStartValue = 1000; //リセットタイマーが作動したときの値を保存する変数
	bool m_flagSwitchOn = false; //スイッチが押されて、透明オブジェクトを持ち上げられる状態になっているかのフラグ
};

