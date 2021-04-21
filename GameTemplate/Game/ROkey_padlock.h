#pragma once
#include "ReversibleObject.h"
#include "OOpadlock.h"

/// <summary>
/// 反転オブジェクト
/// 鍵、南京錠オブジェクト
/// </summary>
class ROkey_padlock : public CReversibleObject
{
public:		//自動で呼ばれるメンバ関数
	bool StartSub() override final;	//スタート関数

	/// <summary>
	/// クエリしてほしいタイミングで呼ばれる関数
	/// </summary>
	void QuerySub()override final;

	CSoundCue* m_padlockbreakSE = nullptr; //padlockbreakSEのサウンドキュー
};

