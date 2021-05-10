#pragma once
#include "ReversibleObject.h"
#include "OOoneway.h"

/// <summary>
/// 反転オブジェクト
/// 左、右オブジェクト
/// </summary>
class ROleft_right : public CReversibleObject
{
public:		//自動で呼ばれるメンバ関数
	bool StartSub() override final;	//スタート関数

	/// <summary>
	/// クエリしてほしいタイミングで呼ばれる関数
	/// </summary>
	void QuerySub()override final;

private:	//privateなメンバ関数
	
	/// <summary>
	/// 当たったオブジェクトを左右反転させる
	/// </summary>
	void ObjectReverse();

private:	//データメンバ
	//一度に複数のオブジェクトを破棄することを防ぐためのフラグ
	bool m_actionFlag = true;	//行動できるか？
};

