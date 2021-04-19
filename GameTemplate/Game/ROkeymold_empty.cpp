#include "stdafx.h"
#include "ROkeymold_empty.h"
#include "ROwire_string.h"

//スタート関数
bool ROkeymold_empty::StartSub()
{
	//初期化用関数
	Init("Assets/modelData/key_mold.tkm", enKeymold,
		"Assets/modelData/key_mold.tkm", enKeymold);

	//OBBのサイズを設定
	Vector3 obbSize;
	obbSize = { 200.0f,200.0f,100.0f };
	//OBBの方向ベクトルの長さを設定
	GetOBB().SetDirectionLength(obbSize);

	return true;
}

/// <summary>
/// クエリしてほしいタイミングで呼ばれる関数
/// </summary>
void ROkeymold_empty::QuerySub()
{
	//自身が「鍵の金型」の時
	if (GetObjectType() == enKeymold)
	{
		//反転オブジェクトの「針金」をクエリ
		QueryLOs<ROwire_string>(enWire, [&](ROwire_string* wire) -> bool
			{
				//自身と「針金」が衝突したら
				if (IsHitObject(*this, *wire))
				{
					//針金を鍵に変える
					wire->ChangeToKey();
				}
				return true;
			}
		);
	}
	//else if (GetObjectType() == enKeymold)
	//{
	//	//QueryLOs<OObigFire>(enBigFire, [&](OObigFire* bigFire) -> bool
	//	//	{
	//	//		if (IsHitObject(*this, *bigFire, hitDot))
	//	//		{
	//	//			bigFire->Damage();
	//	//			DeleteGO(this);
	//	//		}
	//	//		return true;
	//	//	}
	//	//);
	//}
}