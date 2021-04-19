#include "stdafx.h"
#include "ROkey_padlock.h"

//スタート関数
bool ROkey_padlock::StartSub()
{
	//初期化用関数
	Init("Assets/modelData/key.tkm", enKey,
		"Assets/modelData/padlock.tkm", enPadlock);

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
void ROkey_padlock::QuerySub()
{
	//自身が「鍵の金型」の時
	if (GetObjectType() == enKey)
	{
		//障害オブジェクトの「持てない南京錠」をクエリ
		QueryLOs<OOpadlock>(enNotHavePadlock, [&](OOpadlock* padlock) -> bool
			{
				//自身と「持てない南京錠」が衝突したら
				if (IsHitObject(*this, *padlock))
				{
					//「持てない南京錠」を破棄
					DeleteGO(padlock);
					//自身のオブジェクトを破棄
					DeleteGO(this);
				}
				return true;
			}
		);
	}
	//else if (GetObjectType() == enROPadlock)
	//{
	//	QueryLOs<OObigFire>(enBigFire, [&](OObigFire* bigFire) -> bool
	//		{
	//			if (IsHitObject(*this, *bigFire, hitDot))
	//			{
	//				bigFire->Damage();
	//				DeleteGO(this);
	//			}
	//			return true;
	//		}
	//	);
	//}
}