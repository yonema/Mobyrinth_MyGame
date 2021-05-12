#pragma once
#include "Tips.h"
#include "LevelObjectManager.h"

/// <summary>
/// Tipsのコントローラークラス
/// </summary>
class CTipsController : public IGameObject
{
public:		//自動で呼ばれるメンバ関数
	bool Start() override final;	//スタート関数
	void Update() override final;	//アップデート関数

public:		//メンバ関数
	void SetTitleMode()
	{
		m_tips.SetTitleMode();
		this->Deactivate();
	}

private:
	CTips m_tips;		//Tips表示クラス
};

