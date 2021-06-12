#include "stdafx.h"
#include "ROmizu_kori.h"

//スタート関数
bool ROmizu_kori::StartSub()
{
	//初期化用関数
	Init("Assets/modelData/water.tkm", EN_RO_TYPE_WATER,
		"Assets/modelData/fire.tkm", EN_RO_TYPE_FIRE);


	return true;
}

/// <summary>
/// クエリしてほしいタイミングで呼ばれる関数
/// </summary>
void ROmizu_kori::QuerySub()
{
	//自身が「水」の時
	if (GetObjectType() == EN_RO_TYPE_WATER)
	{
		//障害オブジェクトの「でかい火」をクエリ
		QueryLOs<OObigFire>(EN_OO_TYPE_FLAME, [&](OObigFire* bigFire) -> bool
			{
				if (m_actionFlag)
				{
					//自身と「でかい火」が衝突したら
					if (IsHitObject(*this, *bigFire))
					{
						//「でかい火」にダメージを与える
						bigFire->Damage();
						//自身は破棄する
						DeleteGO(this);
						m_actionFlag = false;
					}
				}
				//trueを戻す
				return true;
			}
		);
		//反転オブジェクトの「火」をクエリ
		QueryLOs<ROmizu_kori>(EN_RO_TYPE_FIRE, [&](ROmizu_kori* Fire) -> bool
			{
				//自身と「火」が衝突したら
				if (IsHitObject(*this, *Fire))
				{
					if (m_actionFlag) {
						//相手を破棄する。
						DeleteGO(Fire);
						//自身は破棄する
						DeleteGO(this);
						//行動できなくする
						m_actionFlag = false;
					}
				}
				//trueを戻す
				return true;
			}
		);
	}
	//自身が「火」の時
	else if (GetObjectType() == EN_RO_TYPE_FIRE)
	{
		//反転オブジェクトの「水」をクエリ
		QueryLOs<ROmizu_kori>(EN_RO_TYPE_WATER, [&](ROmizu_kori* Water) -> bool
			{
				//自身と「水」が衝突したら
				if (IsHitObject(*this, *Water))
				{
					if (m_actionFlag) {
						//相手を破棄する。
						DeleteGO(Water);
						//自身は破棄する
						DeleteGO(this);
						//行動できなくする
						m_actionFlag = false;
					}
				}
				//trueを戻す
				return true;
			}
		);
	}
}