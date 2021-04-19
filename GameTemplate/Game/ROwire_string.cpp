#include "stdafx.h"
#include "ROwire_string.h"

//スタート関数
bool ROwire_string::StartSub()
{
	//初期化用関数
	Init("Assets/modelData/wire.tkm", enWire,
		"Assets/modelData/string.tkm", enString);

	//OBBのサイズを設定
	Vector3 obbSize;
	obbSize = { 300.0f,150.0f,100.0f };
	//OBBの方向ベクトルの長さを設定
	GetOBB().SetDirectionLength(obbSize);

	return true;
}

/// <summary>
/// クエリしてほしいタイミングで呼ばれる関数
/// </summary>
void ROwire_string::QuerySub()
{
	//自身が「針金」の時
	if (GetObjectType() == enWire)
	{
		//反転オブジェクトの「鍵の金型」をクエリ
		QueryLOs<ROkeymold_empty>(enKeymold, [&](ROkeymold_empty* keymold) -> bool
			{
				//自身と「鍵の金型」が衝突したら
				if (IsHitObject(*this, *keymold))
				{
					//「鍵」を生成して自身のオブジェクトを破棄する
					ChangeToKey();
				}
				return true;
			}
		);
	}
	//自身が「紐」の時
	else if (GetObjectType() == enString)
	{
		//QueryLOs<OObigFire>(enBigFire, [&](OObigFire* bigFire) -> bool
		//	{
		//		if (IsHitObject(*this, *bigFire, hitDot))
		//		{
		//			bigFire->Damage();
		//			DeleteGO(this);
		//		}
		//		return true;
		//	}
		//);
	}
}

/// <summary>
/// 「鍵」を生成して自身のオブジェクトを破棄する
/// </summary>
void ROwire_string::ChangeToKey()
{
	//「鍵、南京錠」を生成
	ROkey_padlock* RObjrct;
	RObjrct = NewGO<ROkey_padlock>(0, "key_padlock");
	//場所を設定
	RObjrct->SetPosition(m_position);
	//「表」状態（「鍵」状態）に設定
	RObjrct->SetFrontOrBack(ROkey_padlock::enFront);

	//自身のオブジェクトを破棄する
	DeleteGO(this);
}