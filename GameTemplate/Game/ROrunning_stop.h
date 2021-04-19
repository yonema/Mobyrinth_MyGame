#pragma once
#include "ReversibleObject.h"
#include "OOwall.h"

/// <summary>
/// 反転オブジェクト
/// 稼働、停止オブジェクト
/// </summary>
class ROrunning_stop : public CReversibleObject
{
public:		//自動で呼ばれるメンバ関数
	bool StartSub() override final;	//スタート関数

	/// <summary>
	/// クエリしてほしいタイミングで呼ばれる関数
	/// </summary>
	void QuerySub()override final;

	void UpdateSub()override final;	//アップデート関数

private:	//データメンバ
	OOwall* m_pWall = nullptr;		//壁のポインタ
};

