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

public:

	/// <summary>
	/// 壁のポインタを設定する
	/// </summary>
	/// <param name="wall">壁のポインタ</param>
	void SetWall(const OOwall* wall)
	{
		m_pWall = wall;
	}

	/**
	 * @brief 「壁」と衝突した時の処理
	 * @param wall 「壁」のポインタ
	*/
	void HitWall(OOwall* wall);

private:	//データメンバ
	const OOwall* m_pWall = nullptr;		//壁のポインタ
	bool m_actionFlag = true;				//行動できるか？
};

