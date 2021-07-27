#include "stdafx.h"
#include "ROwater_fire.h"

//ReversibleObjectのモデルのファイルパスの定数データを使用可能にする
using namespace ROsFilepathConstdata;

//スタート関数
bool ROwater_fire::StartSub()
{
	//初期化用関数
	Init(MODEL_FILEPATH_WATER, EN_RO_TYPE_WATER,
		MODEL_FILEPATH_FIRE, EN_RO_TYPE_FIRE);


	return true;
}

/// <summary>
/// クエリしてほしいタイミングで呼ばれる関数
/// </summary>
void ROwater_fire::QuerySub()
{
	//自身が「水」の時
	if (GetObjectType() == EN_RO_TYPE_WATER)
	{
		//障害オブジェクトの「炎」をクエリ
		QueryLOs<OOflame>(EN_OO_TYPE_FLAME, [&](OOflame* flame) -> bool
			{
				//自身が「水」で、「炎」と衝突した時の処理
				WaterHitFlame(flame);

				//trueを戻す
				return true;
			}
		);
		//反転オブジェクトの「火」をクエリ
		QueryLOs<ROwater_fire>(EN_RO_TYPE_FIRE, [&](ROwater_fire* fire) -> bool
			{
				//自身が「水」で、「火」と衝突した時の処理
				WaterHitFire(fire);

				//trueを戻す
				return true;
			}
		);
	}
	//自身が「火」の時
	else if (GetObjectType() == EN_RO_TYPE_FIRE)
	{
		//反転オブジェクトの「水」をクエリ
		QueryLOs<ROwater_fire>(EN_RO_TYPE_WATER, [&](ROwater_fire* water) -> bool
			{
				//自身が「火」で、「水」と衝突した時の処理
				FireHitWater(water);

				//trueを戻す
				return true;
			}
		);
	}

	return;
}

/**
 * @brief 自身が「水」で、「炎」と衝突した時の処理
 * @param flame [in] 「炎」のポインタ
*/
void ROwater_fire::WaterHitFlame(OOflame* flame)
{
	//行動可能か？
	if (m_actionFlag)
	{
		//自身と「炎」が衝突したら
		if (IsHitLevelObject(*this, *flame))
		{
			//「炎」にダメージを与える
			flame->Damage();
			//自身は破棄する
			DeleteGO(this);
			//行動できなくする
			m_actionFlag = false;
		}
	}

	return;
}

/**
 * @brief 自身が「水」で、「火」と衝突した時の処理
 * @param fire [in] 「火」のポインタ
*/
void ROwater_fire::WaterHitFire(ROwater_fire* fire)
{
	//行動できるか？
	if (m_actionFlag)
	{
		//自身と「火」が衝突したら
		if (IsHitLevelObject(*this, *fire))
		{
			//相手を破棄する。
			DeleteGO(fire);
			//自身は破棄する
			DeleteGO(this);
			//行動できなくする
			m_actionFlag = false;
		}
	}

	return;
}

/**
 * @brief 自身が「火」で、「水」と衝突した時の処理
 * @param water [in] 「水」のポインタ
*/
void ROwater_fire::FireHitWater(ROwater_fire* water)
{
	//行動できるか？
	if (m_actionFlag) 
	{
		//自身と「水」が衝突したら
		if (IsHitLevelObject(*this, *water))
		{
			//相手を破棄する。
			DeleteGO(water);
			//自身は破棄する
			DeleteGO(this);
			//行動できなくする
			m_actionFlag = false;
		}
	}

	return;
}