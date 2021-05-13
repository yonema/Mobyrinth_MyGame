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
			//オブジェクトを持っている場合
			levelObjects[i]->SetFlagHeldPlayer(false);
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
		}
	}

private: //メンバ変数
	float m_resetTimer = 0.0f; //オブジェクトの状態をリセットするまでのカウントに使用するリセットタイマー
	float m_resetTimerStartValue = 10.0f; //リセットタイマーが作動したときの値を保存する変数
	bool m_flagSwitchOn = false; //スイッチが押されて、透明オブジェクトを持ち上げられる状態になっているかのフラグ



};

