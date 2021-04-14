#include "stdafx.h"
#include "ROmizu_kori.h"

//スタート関数
bool ROmizu_kori::StartSub()
{
	//初期化用関数
	Init("Assets/modelData/water.tkm", enWater,
		"Assets/modelData/ice.tkm", enIce);

	return true;
}

/// <summary>
/// クエリしてほしいタイミングで呼ばれる関数
/// </summary>
void ROmizu_kori::QuerySub()
{
	//自身が「水」の時
	if (GetObjectType() == enWater)
	{
		//障害オブジェクトの「でかい火」をクエリ
		QueryLOs<OObigFire>(enBigFire, [&](OObigFire* bigFire) -> bool
			{
				//自身と「でかい火」が衝突したら
				if (IsHitObject(*this, *bigFire))
				{
					//「でかい火」にダメージを与える
					bigFire->Damage();
					//自身は破棄する
					DeleteGO(this);
				}
				//trueを戻す
				return true;
			}
		);
	}
	//自身が「氷」の時
	else if (GetObjectType() == enIce)
	{
		//障害オブジェクトの「でかい火」をクエリ
		QueryLOs<OObigFire>(enBigFire, [&](OObigFire* bigFire) -> bool
			{
				//自身と「でかい火」が衝突したら
				if (IsHitObject(*this, *bigFire))
				{
					//「でかい火」にダメージを与える
					bigFire->Damage();
					//自身は破棄する
					DeleteGO(this);
				}
				//trueを戻す
				return true;
			}
		);
	}
}