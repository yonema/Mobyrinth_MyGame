#pragma once
#include "ReversibleObject.h"
#include "ROkeymold_empty.h"
#include "ROkey_padlock.h"

/// <summary>
/// 反転オブジェクト
/// 紐、針金オブジェクト
/// </summary>
class ROwire_string : public CReversibleObject
{
public:		//自動で呼ばれるメンバ関数
	bool StartSub() override final;	//スタート関数

	/// <summary>
	/// クエリしてほしいタイミングで呼ばれる関数
	/// </summary>
	void QuerySub()override final;
public:		//メンバ関数

	/// <summary>
	/// 「鍵」を生成して自身のオブジェクトを破棄する
	/// </summary>
	void ChangeToKey();
};

