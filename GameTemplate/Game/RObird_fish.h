#pragma once
#include "ReversibleObject.h"

/// <summary>
/// 反転オブジェクト。
/// 水、氷オブジェクト
/// </summary>
class RObird_fish : public CReversibleObject
{
public:		//自動で呼ばれるメンバ関数
	bool StartSub() override final;				//スタート関数
	~RObird_fish();								//デストラクタ
	void QuerySub()override final;				//クエリしてほしいタイミングで呼ばれる関数
	void UpdateSub()override final;				//アップデート関数
	void SetFrontOrBackSub()override final;		//表と裏をセットするサブ関数

private:	//データメンバ

	/// <summary>
	/// もう一つの裏表のオブジェクトのモデルレンダラー
	/// </summary>
	CModelRender* m_otherModelRender[enFrontAndBackNum] = { nullptr };
	int m_reversibleType[enFrontAndBackNum] = { enEnpty };				//オブジェクトのタイプ
};

