#pragma once
#include "ReversibleObject.h"

/// <summary>
/// 反転オブジェクト
/// 鍵の金型、空っぽオブジェクト
/// </summary>
class ROkeymold_empty : public CReversibleObject
{
public:		//自動で呼ばれるメンバ関数
	bool StartSub() override final;	//スタート関数

	/// <summary>
	/// クエリしてほしいタイミングで呼ばれる関数
	/// </summary>
	void QuerySub()override final;
};

