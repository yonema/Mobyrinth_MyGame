#pragma once
#include "ReversibleObject.h"
#include "OOflame.h"

/// <summary>
/// 反転オブジェクト。
/// 水、火オブジェクト
/// </summary>
class ROwater_fire : public CReversibleObject
{
public:		//自動で呼ばれるメンバ関数
	bool StartSub() override final;	//スタート関数

	/// <summary>
	/// クエリしてほしいタイミングで呼ばれる関数
	/// </summary>
	void QuerySub()override final;

private:	//privateなメンバ関数

	/**
	 * @brief 自身が「水」で、「炎」と衝突した時の処理
	 * @param flame 「炎」のポインタ
	*/
	void WaterHitFlame(OOflame* flame);

	/**
	 * @brief 自身が「水」で、「火」と衝突した時の処理
	 * @param fire 「火」のポインタ
	*/
	void WaterHitFire(ROwater_fire* fire);

	/**
	 * @brief 自身が「火」で、「水」と衝突した時の処理
	 * @param water 「水」のポインタ
	*/
	void FireHitWater(ROwater_fire* water);

private:	//データメンバ
	//一度に複数のオブジェクトを破棄することを防ぐためのフラグ
	bool m_actionFlag = true;	//行動できるか？
};

