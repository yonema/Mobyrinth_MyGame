#pragma once
#include "ReversibleObject.h"
#include "OOflame.h"

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
	//一度に複数のオブジェクトを破棄することを防ぐためのフラグ
	bool m_actionFlag = true;	//行動できるか？
};

