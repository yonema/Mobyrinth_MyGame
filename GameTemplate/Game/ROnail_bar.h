#pragma once
#include "ReversibleObject.h"
#include "OObox.h"
#include "OOpadlock.h"

/// <summary>
/// 反転オブジェクト
/// 釘、バールオブジェクト
/// </summary>
class ROnail_bar : public CReversibleObject
{
public:		//自動で呼ばれるメンバ関数
	bool StartSub() override final;	//スタート関数

	~ROnail_bar();					//デストラクタ

	/// <summary>
	/// クエリしてほしいタイミングで呼ばれる関数
	/// </summary>
	void QuerySub()override final;

private:	//データメンバ
	//一度に複数のオブジェクトを破棄することを防ぐためのフラグ
	bool m_actionFlag = true;			//行動できるか？
	CSoundCue* m_brokenSE = nullptr;	//壊れた時のサウンド
};

