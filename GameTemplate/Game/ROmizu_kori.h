#pragma once
#include "ReversibleObject.h"
#include "OObigFire.h"

/// <summary>
/// 反転オブジェクト。
/// 水、火オブジェクト
/// </summary>
class ROmizu_kori : public CReversibleObject
{
public:		//自動で呼ばれるメンバ関数
	bool StartSub() override final;	//スタート関数

	/// <summary>
	/// クエリしてほしいタイミングで呼ばれる関数
	/// </summary>
	void QuerySub()override final;

private:	//データメンバ

};

