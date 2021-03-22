#pragma once
#include "ReversibleObject.h"
#include "OObox.h"

/// <summary>
/// 反転オブジェクト。
/// 斧、つるはしオブジェクト
/// </summary>
class ROaxe_pickaxe : public CReversibleObject
{
public:		//自動で呼ばれるメンバ関数
	bool StartSub() override final;	//スタート関数

	/// <summary>
	/// クエリしてほしいタイミングで呼ばれる関数
	/// </summary>
	void QuerySub()override final;

private:	//データメンバ

};

